#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <sstream>
#include <thread>

#include <benchmark/indicators.hpp>

template <class Fn>
class benchmark {

  std::size_t num_iterations_{0};
  std::size_t max_num_runs_{0};

  auto estimate_execution_time(Fn fn) {
    using namespace std::chrono;

    long double result;
    bool first_run{true};
    std::size_t warmup_runs = 3;
    for (std::size_t i = 0; i < warmup_runs; i++) {
      const auto start = steady_clock::now();
      fn();
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

  void update_iterations(Fn fn) {
    const auto early_estimate_execution_time = estimate_execution_time(fn);

    num_iterations_ = 10; // fixed
    const auto min_runs = 10;
    const auto min_benchmark_time = early_estimate_execution_time * min_runs * num_iterations_;
    const auto one_minute = 6E10;

    const auto benchmark_time = std::max(double(min_benchmark_time), one_minute);
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

  benchmark(const std::string& name, Fn fn) {
    using namespace std::chrono;

    using namespace indicators;
    ProgressSpinner spinner{
      option::PrefixText{"[" + name + "]"},
      option::ForegroundColor{Color::white},
      option::SpinnerStates{std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"}},
      option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
      option::ShowSpinner{false},
      option::ShowElapsedTime{true},
      option::ShowRemainingTime{true}
    };

    spinner.set_option(option::MaxProgress{max_num_runs_});
    update_iterations(fn);
    spinner.set_option(option::MaxProgress{max_num_runs_});

    long double lowest_rsd = 100;
    std::size_t num_iterations_lowest_rsd = 0;
    long double mean_lowest_rsd = 0;

    std::size_t num_runs = 0;
    spinner.set_progress(num_runs);

    std::array<long double, 10> durations;

    // Hide cursor
    indicators::show_console_cursor(false);

    while(true) {
      // Benchmark runs
      for (std::size_t i = 0; i < num_iterations_; i++) {
        const auto start = high_resolution_clock::now();
        fn();
        const auto end = high_resolution_clock::now();
        const auto execution_time = duration_cast<std::chrono::nanoseconds>(end - start).count();
        durations[i] = execution_time;
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

      // Save record of lowest RSD
      const auto current_lowest_rsd = lowest_rsd;
      lowest_rsd = std::min(relative_standard_deviation, lowest_rsd);
      if (lowest_rsd < current_lowest_rsd) {
        num_iterations_lowest_rsd = num_iterations_;
        mean_lowest_rsd = mean;
      }

      spinner.set_progress(num_runs);

      // Show iteration as postfix text
      std::stringstream os;
      os
        // << num_runs << "/" << max_num_runs_ << " "
        << std::setprecision(3)
        << "μ = "
        << duration_to_string(mean_lowest_rsd) 
        << " ± " << lowest_rsd << "%";
      spinner.set_option(option::PostfixText{os.str()});

      if (num_runs >= max_num_runs_) {
        break;
      }

      num_runs += 1;
    }

    std::stringstream os;
    os
      << std::setprecision(3)
      << "μ = "
      << duration_to_string(mean_lowest_rsd) 
      << " ± " << lowest_rsd << "%";

    spinner.set_option(option::ForegroundColor{Color::green});
    spinner.set_option(option::PrefixText{"✔ " + name});
    spinner.set_option(option::ShowSpinner{false});
    spinner.set_option(option::ShowPercentage{false});
    spinner.set_option(option::PostfixText{os.str()});
    spinner.mark_as_completed();

    // Show cursor
    indicators::show_console_cursor(true);
  }
};