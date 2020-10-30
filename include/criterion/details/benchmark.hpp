#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <csignal>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <string>
#include <sstream>
#include <thread>
#include <utility>

#include <criterion/details/benchmark_config.hpp>
#include <criterion/details/benchmark_result.hpp>
#include <criterion/details/indicators.hpp>

class benchmark {
  benchmark_config config_;
  using Fn = benchmark_config::Fn;

  std::size_t warmup_runs_{3};
  std::size_t num_iterations_{0};
  std::size_t max_num_runs_{0};

  long double estimate_minimum_measurement_cost() {
    using namespace std::chrono;
    std::vector<long double> durations;

    for (std::size_t i = 0; i < 10; i++) {
      const auto start = steady_clock::now();
      // do nothing
      const auto end = steady_clock::now();
      const auto execution_time = static_cast<long double>(duration_cast<std::chrono::nanoseconds>(end - start).count());
      durations.push_back(execution_time);
    }
    return *std::min_element(durations.begin(), durations.end());
  }

  long double estimate_execution_time() {
    using namespace std::chrono;

    long double result;
    bool first_run{true};
    for (std::size_t i = 0; i < warmup_runs_; i++) {
      std::chrono::steady_clock::time_point start_timestamp;
      std::optional<std::chrono::steady_clock::time_point> teardown_timestamp;
      const auto start = steady_clock::now();
      config_.fn(start_timestamp, teardown_timestamp, config_.parameters);
      const auto end = steady_clock::now();
      const auto execution_time = static_cast<long double>(duration_cast<std::chrono::nanoseconds>(end - start).count());
      if (first_run) {
        result = execution_time;
        first_run = false;
      }
      else {
        result = std::min(execution_time, result);
      }
    }
    return result;
  }

  void update_iterations() {
    const auto early_estimate_execution_time = estimate_execution_time();

    num_iterations_ = 10; // fixed
    const auto min_runs = 10;
    const auto min_benchmark_time = early_estimate_execution_time * min_runs * num_iterations_;
    const auto ten_seconds = 1e+10;

    const auto benchmark_time = std::max(double(min_benchmark_time), ten_seconds);
    const auto total_iterations = size_t(benchmark_time) / early_estimate_execution_time;

    max_num_runs_ = std::max(size_t(total_iterations / num_iterations_), size_t(min_runs));

    max_num_runs_ = std::min(max_num_runs_, size_t(1E7)); // no more than 1E7 runs, don't need it
  }

  std::string duration_to_string(const long double& ns) {
    std::stringstream os;
    if (ns < 1E3) {
      os << std::setprecision(3) << ns << "ns";
    }
    else if (ns < 1E6) {
      os << std::setprecision(3) << (ns / 1E3) << "us";
    }
    else if (ns < 1E9) {
      os << std::setprecision(3) << (ns / 1E6) << "ms";
    }
    else {
      os << std::setprecision(3) << (ns / 1E9) << "s";
    }
    return os.str();
  }

public:

  benchmark(const benchmark_config& config): 
    config_(config) {}

  static inline std::map<std::string, benchmark_result> results;

  void run() {

    using namespace std::chrono;

    // run empty function to estimate minimum delay in scheduling and executing user function
    const auto estimated_minimum_measurement_cost = estimate_minimum_measurement_cost();

    const std::string prefix = config_.name + config_.parameterized_instance_name + " ";
    // std::cout << termcolor::bold << termcolor::yellow << prefix << termcolor::reset << "\n";

    using namespace indicators;

    show_console_cursor(false);

    // Get an early estimate for execution time
    // Update number of iterations to run for this benchmark based on estimate
    update_iterations();

    ProgressBar spinner{
      option::PrefixText{prefix},
      option::BarWidth{10},
      option::Lead{"■"},
      option::Fill{"■"},
      option::Remainder{"-"},
      option::ForegroundColor{Color::white},
      option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
      // option::ShowSpinner{false},
      option::ShowElapsedTime{true},
      option::ShowRemainingTime{true}
    };

    spinner.set_option(option::MaxProgress{max_num_runs_});

    long double lowest_rsd = 100;
    long double best_estimate_mean = 0;
    bool first_run{true};

    long double overall_best_execution_time = 0;
    long double overall_worst_execution_time = 0;

    std::size_t num_runs = 0;
    spinner.set_progress(num_runs);

    std::array<long double, 10> durations;

    while(true) {
      // Benchmark runs
      for (std::size_t i = 0; i < num_iterations_; i++) {
        std::optional<std::chrono::steady_clock::time_point> teardown_timestamp;
        auto start = steady_clock::now();
        config_.fn(start, teardown_timestamp, config_.parameters);
        auto end = steady_clock::now();
        if (teardown_timestamp)
          end = teardown_timestamp.value();
        const auto execution_time = duration_cast<std::chrono::nanoseconds>(end - start).count();
        durations[i] = std::abs(execution_time - estimated_minimum_measurement_cost);
      }
      auto size = num_iterations_;
      const long double mean = std::accumulate(durations.begin(), durations.end(), 0.0) / size;

      long double E = 0;
      for (std::size_t i = 0; i < size; i++) {
        E += std::pow(durations[i] - mean, 2);
      }
      const long double variance = E / size;
      const long double standard_deviation = std::sqrt(variance);
      const long double relative_standard_deviation = standard_deviation * 100 / mean;

      if (first_run) {
        lowest_rsd = relative_standard_deviation;
        best_estimate_mean = mean;
        overall_best_execution_time = *std::min_element(durations.begin(), durations.end());
        overall_worst_execution_time = *std::max_element(durations.begin(), durations.end());
        first_run = false;
      }
      else {
        // Save record of lowest RSD
        const auto current_lowest_rsd = lowest_rsd;
        lowest_rsd = std::min(relative_standard_deviation, lowest_rsd);
        if (lowest_rsd < current_lowest_rsd) {
          // There's a new LOWEST relative standard deviation

          if (mean < best_estimate_mean) {
            best_estimate_mean = mean; // new mean is lower
          } 
          else {
            lowest_rsd = current_lowest_rsd; // go back to old estimate
          }
        } else {
          lowest_rsd = current_lowest_rsd; // go back to old estimate
        }

        // Save best and worst duration
        overall_best_execution_time = std::min(overall_best_execution_time, *std::min_element(durations.begin(), durations.end()));
        overall_worst_execution_time = std::max(overall_worst_execution_time, *std::min_element(durations.begin(), durations.end()));
      }

      spinner.set_progress(num_runs);

      // Show iteration as postfix text
      // std::stringstream os;
      // os << "[" << num_runs * num_iterations_ << "/" << max_num_runs_ * num_iterations_ << "]";
      // spinner.set_option(option::PostfixText{os.str()});

      if (num_runs >= max_num_runs_) {
        break;
      }

      num_runs += 1;
    }

    std::cout 
      << termcolor::bold 
      << termcolor::green 
      << std::setprecision(3)
      << "    "
      << duration_to_string(best_estimate_mean) 
      << " ± " << lowest_rsd << "%"
      << " {Best: "
      << duration_to_string(overall_best_execution_time) << ", Worst: "
      << duration_to_string(overall_worst_execution_time)
      << "}\n\n"
      << termcolor::reset;

    results.insert(std::make_pair(
      prefix,
      benchmark_result {
        .name = config_.name + config_.parameterized_instance_name,
        .num_warmup_runs = warmup_runs_,
        .num_runs = max_num_runs_,
        .num_iterations = num_iterations_,
        .best_estimate_mean = best_estimate_mean,
        .best_estimate_rsd = lowest_rsd,
        .overall_best_execution_time = overall_best_execution_time,
        .overall_worst_execution_time = overall_worst_execution_time
      }
    ));

    indicators::show_console_cursor(true);
  }
};