#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <csignal>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include <criterion/details/benchmark_config.hpp>
#include <criterion/details/benchmark_result.hpp>
#include <criterion/details/console_writer.hpp>
#include <criterion/details/indicators.hpp>

namespace criterion {

class benchmark {
  benchmark_config config_;
  using Fn = benchmark_config::Fn;

  static inline constexpr std::size_t num_iterations_{20};
  std::size_t max_num_runs_{0};
  const long double ten_seconds_{1e+10};
  long double min_benchmark_time_{ten_seconds_};
  long double benchmark_time_;
  long double early_estimate_execution_time_;

  long double estimate_minimum_measurement_cost() {
    using namespace std::chrono;
    std::vector<long double> durations;

    for (std::size_t i = 0; i < 10; i++) {
      const auto start = steady_clock::now();
      // do nothing
      const auto end = steady_clock::now();
      const auto execution_time =
          static_cast<long double>(duration_cast<std::chrono::nanoseconds>(end - start).count());
      durations.push_back(execution_time);
    }
    return *std::min_element(durations.begin(), durations.end());
  }

  long double estimate_execution_time() {
    using namespace std::chrono;

    long double result;
    bool first_run{true};
    for (std::size_t i = 0; i < warmup_runs; i++) {
      std::chrono::steady_clock::time_point start_timestamp;
      std::optional<std::chrono::steady_clock::time_point> teardown_timestamp;
      const auto start = steady_clock::now();
      config_.fn(start_timestamp, teardown_timestamp, config_.parameters);
      const auto end = steady_clock::now();
      const auto execution_time =
          static_cast<long double>(duration_cast<std::chrono::nanoseconds>(end - start).count());
      if (first_run) {
        result = execution_time;
        first_run = false;
      } else {
        result = std::min(execution_time, result);
      }
    }
    return result;
  }

  void update_iterations() {
    early_estimate_execution_time_ = estimate_execution_time();

    if (early_estimate_execution_time_ < 1)
      early_estimate_execution_time_ = 1;

    auto min_runs = 2;

    if (early_estimate_execution_time_ <= 100) {              // 100ns
      benchmark_time_ = 5e+8;                                 // 500 ms
    } else if (early_estimate_execution_time_ <= 1000) {      // 1us
      benchmark_time_ = 1e+9;                                 // 1s
    } else if (early_estimate_execution_time_ <= 100000) {    // 100us
      benchmark_time_ = 2.5e+9;                               // 2.5s
    } else if (early_estimate_execution_time_ <= 1000000) {   // 1ms
      benchmark_time_ = 5e+9;                                 // 5s
    } else if (early_estimate_execution_time_ <= 100000000) { // 100ms
      benchmark_time_ = 7.5e+9;                               // 7.5s
    } else {
      benchmark_time_ = min_benchmark_time_;
    }

    benchmark_time_ =
        std::max(early_estimate_execution_time_ * min_runs * num_iterations_, benchmark_time_);
    const auto total_iterations = size_t(benchmark_time_) / early_estimate_execution_time_;
    max_num_runs_ = std::max(size_t(total_iterations / num_iterations_), size_t(min_runs));
  }

public:
  benchmark(const benchmark_config &config) : config_(config) {}

  static inline std::unordered_map<std::string, benchmark_result> results;
  static inline std::vector<std::string> benchmark_execution_order;
  static inline bool show_console_output = true;
  static inline std::size_t warmup_runs = 3;

  void run() {
    std::chrono::steady_clock::time_point benchmark_start_timestamp;

    using namespace std::chrono;

    // run empty function to estimate minimum delay in scheduling and executing user function
    const auto estimated_minimum_measurement_cost = estimate_minimum_measurement_cost();

    const std::string benchmark_instance_name = config_.name + config_.parameterized_instance_name;
    benchmark_execution_order.push_back(benchmark_instance_name);

    // Get an early estimate for execution time
    // Update number of iterations to run for this benchmark based on estimate
    update_iterations();

    const auto total_number_of_iterations = max_num_runs_ * num_iterations_;

    long double lowest_rsd = 100;
    long double lowest_rsd_mean = 0;
    std::size_t lowest_rsd_index = 0;
    bool first_run{true};

    long double fastest_execution_time = 0;
    long double slowest_execution_time = 0;
    std::vector<long double> mean_in_each_run{};
    mean_in_each_run.reserve(max_num_runs_);

    std::size_t num_runs = 0;
    std::array<long double, num_iterations_> durations;

    using namespace indicators;

    // Hide cursor
    if (show_console_output) {
      show_console_cursor(false);
    }

    BlockProgressBar bar{option::BarWidth{50},
                         option::Start{"["},
                         option::End{"]"},
                         option::ShowElapsedTime{true},
                         option::ShowRemainingTime{true},
                         option::ShowPercentage{true},
                         option::ForegroundColor{Color::white},
                         option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
                         option::MaxProgress{total_number_of_iterations},
                         option::PrefixText{" > " + benchmark_instance_name + " "}};

    while (true) {
      // Benchmark runs
      if (first_run) {
        benchmark_start_timestamp = std::chrono::steady_clock::now();
      }
      for (std::size_t i = 0; i < num_iterations_; i++) {
        std::optional<std::chrono::steady_clock::time_point> teardown_timestamp;
        auto start = steady_clock::now();
        config_.fn(start, teardown_timestamp, config_.parameters);
        auto end = steady_clock::now();
        if (teardown_timestamp)
          end = teardown_timestamp.value();
        const auto execution_time = duration_cast<std::chrono::nanoseconds>(end - start).count();
        durations[i] = std::abs(execution_time - estimated_minimum_measurement_cost);
        if (show_console_output) {
          bar.tick();
        }
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

      const auto mean_in_this_run =
          std::accumulate(durations.begin(), durations.end(), 0.0) / num_iterations_;
      mean_in_each_run.push_back(mean_in_this_run);

      if (first_run) {
        lowest_rsd = relative_standard_deviation;
        lowest_rsd_mean = mean;
        lowest_rsd_index = num_runs + 1;
        fastest_execution_time = *std::min_element(durations.begin(), durations.end());
        slowest_execution_time = *std::max_element(durations.begin(), durations.end());
        first_run = false;
      } else {
        // Save record of lowest RSD
        const auto current_lowest_rsd = lowest_rsd;
        const auto current_lowest_rsd_index = lowest_rsd_index;
        lowest_rsd = std::min(relative_standard_deviation, lowest_rsd);
        if (lowest_rsd < current_lowest_rsd) {
          // There's a new LOWEST relative standard deviation

          if (mean < lowest_rsd_mean) {
            lowest_rsd_mean = mean; // new mean is lower
            lowest_rsd_index = num_runs + 1;
          } else {
            lowest_rsd = current_lowest_rsd; // go back to old estimate
            lowest_rsd_index = current_lowest_rsd_index;
          }
        } else {
          lowest_rsd = current_lowest_rsd; // go back to old estimate
          lowest_rsd_index = current_lowest_rsd_index;
        }

        // Save best and worst duration
        const auto current_best_execution_time =
            *std::min_element(durations.begin(), durations.end());
        if (current_best_execution_time > 0)
          fastest_execution_time = std::min(fastest_execution_time, current_best_execution_time);

        const auto current_worst_execution_time =
            *std::max_element(durations.begin(), durations.end());
        slowest_execution_time = std::max(slowest_execution_time, current_worst_execution_time);
      }

      num_runs += 1;

      if (num_runs >= max_num_runs_) {
        break;
      }

      const auto now = std::chrono::steady_clock::now();
      const auto elapsed_time =
          std::chrono::duration_cast<std::chrono::nanoseconds>(now - benchmark_start_timestamp)
              .count();
      if (elapsed_time > benchmark_time_) {
        break;
      } else {
        if (show_console_output) {
          const auto percentage_completed = elapsed_time / benchmark_time_;
          const auto new_bar_progress = percentage_completed * total_number_of_iterations;
          bar.set_progress(new_bar_progress);
        }
      }
    }

    const auto mean_execution_time =
        (std::accumulate(mean_in_each_run.begin(), mean_in_each_run.end(), 0.0) / num_runs);

    const auto benchmark_result = criterion::benchmark_result{
        .name = benchmark_instance_name,
        .num_warmup_runs = warmup_runs,
        .num_runs = max_num_runs_,
        .num_iterations = num_iterations_,
        .lowest_rsd = lowest_rsd,
        .lowest_rsd_mean = lowest_rsd_mean,
        .lowest_rsd_index = lowest_rsd_index,
        .warmup_execution_time = early_estimate_execution_time_,
        .mean_execution_time = mean_execution_time,
        .fastest_execution_time = fastest_execution_time,
        .slowest_execution_time = slowest_execution_time,
        .average_iteration_performance = (1E9 / mean_execution_time),
        .fastest_iteration_performance = (1E9 / fastest_execution_time),
        .slowest_iteration_performance = (1E9 / slowest_execution_time)};

    results.insert(std::make_pair(benchmark_instance_name, benchmark_result));

    if (show_console_output) {
      bar.set_progress(total_number_of_iterations);
      bar.mark_as_completed();

      // Show console cursor
      show_console_cursor(true);

      console_writer::write_result(benchmark_result);
    }
  }
};

} // namespace criterion