#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

#include "indicators.hpp"

template <class Fn>
class benchmark {

  std::size_t num_iterations_{0};
  std::size_t max_num_runs_{0};

  auto estimate_execution_time(Fn fn) {
    using namespace std::chrono;

    long double result;
    bool first_run{true};
    std::size_t warmup_runs = 10;
    for (std::size_t i = 0; i < warmup_runs; i++) {
      const auto start = high_resolution_clock::now();
      fn();
      const auto end = high_resolution_clock::now();
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

    if (early_estimate_execution_time < 100) {
      // tens of nanoseconds
      num_iterations_ = 128000;
      max_num_runs_ = 100000;
    }
    else if (early_estimate_execution_time < 1000) {
      // hundreds of nanoseconds
      num_iterations_ = 64000;
      max_num_runs_ = 10000;
    }
    else if (early_estimate_execution_time < 1000000) {
      // microseconds
      num_iterations_ = 32000;
      max_num_runs_ = 1000;
    }
    else if (early_estimate_execution_time < 1000000000) {
      // milliseconds
      num_iterations_ = 4000;
      max_num_runs_ = 100;
    }
    else {
      // seconds
      num_iterations_ = 1000;
      max_num_runs_ = 10;
    }
  }

  std::string duration_to_string(const long double& ns) {
    std::stringstream os;
    if (ns < 1E3) {
      os << ns << "ns";
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

  benchmark(Fn fn) {
    using namespace std::chrono;
    using namespace indicators;

    update_iterations(fn);

    // Hide cursor
    indicators::show_console_cursor(false);

    BlockProgressBar bar{
      option::BarWidth{20},
      option::ForegroundColor{Color::white},
      option::FontStyles{
            std::vector<FontStyle>{FontStyle::bold}},
      option::MaxProgress{max_num_runs_}
    };

    long double lowest_rsd = 100;
    std::size_t num_iterations_lowest_rsd = 0;
    long double mean_lowest_rsd = 0;

    std::size_t num_runs = 1;

    std::vector<long double> durations;
    durations.reserve(num_iterations_);

    while(true) {

      update_iterations(fn);
      bar.set_option(option::MaxProgress{max_num_runs_});

      const auto warmup = num_iterations_ * 0.2; // 20%

      // Warm-up runs
      for (std::size_t i = 0; i < warmup; i++) {
        fn();
      }

      // Benchmark runs
      for (std::size_t i = warmup; i < num_iterations_; i++) {
        const auto start = high_resolution_clock::now();
        fn();
        const auto end = high_resolution_clock::now();
        const auto execution_time = duration_cast<std::chrono::nanoseconds>(end - start).count();
        durations.push_back(execution_time);
      }
      auto size = num_iterations_ - warmup;
      const long double mean = std::accumulate(durations.begin(), durations.end(), 0.0) / size;

      long double E = 0;
      for (std::size_t i = 0; i < size; i++) {
        E += std::pow(durations.at(i) - mean, 2);
      }
      const long double variance = E / size;
      const long double standard_deviation = std::sqrt(variance);
      const long double relative_standard_deviation = standard_deviation * 100 / mean;

      // Save record of lowest RSD
      const auto current_lowest_rsd = lowest_rsd;
      lowest_rsd = std::min(relative_standard_deviation, lowest_rsd);
      if (lowest_rsd < current_lowest_rsd && num_iterations_ >= num_iterations_lowest_rsd) {
        num_iterations_lowest_rsd = num_iterations_;
        mean_lowest_rsd = mean;
      }

      durations.clear();
      num_runs += 1;

      std::stringstream os;
      os << num_runs << "/" << max_num_runs_
         << std::setprecision(3)
         << " | Current Estimate: μ = " 
         << duration_to_string(mean_lowest_rsd) << " ± " << lowest_rsd
         << "%, N = " << num_iterations_lowest_rsd;

      // Show iteration as postfix text
      bar.set_option(option::PostfixText{
        os.str()
      });

      if (num_runs + 1 == max_num_runs_) {
        // next is last
        if (lowest_rsd > 10) { // too high, not reliable
          num_runs = 0;
          lowest_rsd = 100;
          num_iterations_lowest_rsd = 0;
          mean_lowest_rsd = 0;
        }
      }

      // update progress bar
      bar.set_progress(num_runs);

      if (num_runs == max_num_runs_) {
        break;
      }
    }

    std::stringstream os;
    os << num_runs << "/" << max_num_runs_
        << std::setprecision(3)
        << " | Best Estimate: μ = " 
        << mean_lowest_rsd << "ns ± " << lowest_rsd
        << "%, N = " << num_iterations_lowest_rsd;

    // Show iteration as postfix text
    bar.set_option(option::PostfixText{
      os.str()
    });

    // Show cursor
    show_console_cursor(true);
  }

  ~benchmark() {
    indicators::show_console_cursor(true);
  }
};