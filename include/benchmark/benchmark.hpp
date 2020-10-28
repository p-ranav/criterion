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
#include <string>
#include <sstream>
#include <thread>
#include <utility>

#include <benchmark/indicators.hpp>

#include <string.h>

#if defined(_WIN32)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

struct benchmark_config {
  std::string name;
  std::function<void()> fn;
};

class benchmark {

  std::string name_{""};
  using Fn = std::function<void()>;
  Fn fn_;

  std::size_t warmup_runs_{3};
  std::size_t num_iterations_{0};
  std::size_t max_num_runs_{0};

  long double estimate_measurement_error() {
    using namespace std::chrono;
    std::vector<long double> durations;

    for (std::size_t i = 0; i < 10; i++) {
      const auto start = steady_clock::now();
      // do nothing
      const auto end = steady_clock::now();
      const auto execution_time = static_cast<long double>(duration_cast<std::chrono::nanoseconds>(end - start).count());
      durations.push_back(execution_time);
    }
    return std::accumulate(durations.begin(), durations.end(), 0.0) / durations.size();
  }

  long double estimate_execution_time(Fn fn) {
    using namespace std::chrono;

    long double result;
    bool first_run{true};
    for (std::size_t i = 0; i < warmup_runs_; i++) {
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

  benchmark(const std::string& name, Fn fn): 
    name_(name), 
    fn_(fn) {

    using namespace std::chrono;

    // run empty function to estimate minimum delay in scheduling and executing user function
    const auto estimated_measurement_error = estimate_measurement_error();

    const std::string prefix = name_;
    std::cout << termcolor::bold << termcolor::yellow << prefix << termcolor::reset << "\n";

    using namespace indicators;

    show_console_cursor(false);

    ProgressSpinner spinner{
      option::PrefixText{"  "},
      option::ForegroundColor{Color::white},
      option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
      option::ShowSpinner{false},
      option::ShowElapsedTime{true},
      option::ShowRemainingTime{true}
    };

    spinner.set_option(option::MaxProgress{max_num_runs_});
    update_iterations(fn_);
    spinner.set_option(option::MaxProgress{max_num_runs_});

    long double lowest_rsd = 100;
    long double mean_lowest_rsd = 0;
    bool first_run{true};

    std::size_t num_runs = 0;
    spinner.set_progress(num_runs);

    std::array<long double, 10> durations;

    while(true) {
      // Benchmark runs
      for (std::size_t i = 0; i < num_iterations_; i++) {
        const auto start = high_resolution_clock::now();
        fn_();
        const auto end = high_resolution_clock::now();
        const auto execution_time = duration_cast<std::chrono::nanoseconds>(end - start).count();
        durations[i] = std::abs(execution_time - estimated_measurement_error);
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
        mean_lowest_rsd = mean;
        first_run = false;
      }
      else {
        // Save record of lowest RSD
        const auto current_lowest_rsd = lowest_rsd;
        lowest_rsd = std::min(relative_standard_deviation, lowest_rsd);
        if (lowest_rsd < current_lowest_rsd) {
          // There's a new LOWEST relative standard deviation

          if (mean < mean_lowest_rsd) {
            mean_lowest_rsd = mean; // new mean is lower
          } 
          else {
            lowest_rsd = current_lowest_rsd; // go back to old estimate
          }
        } else {
          lowest_rsd = current_lowest_rsd; // go back to old estimate
        }
      }

      spinner.set_progress(num_runs);

      // Show iteration as postfix text
      std::stringstream os;
      os
        << std::setprecision(3)
        << "μ = "
        << duration_to_string(mean_lowest_rsd) 
        << " ± " << lowest_rsd << "%"
        << " [N = " << max_num_runs_ * num_iterations_ << "]";
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
      << " ± " << lowest_rsd << "%"
      << " [N = " << max_num_runs_ * num_iterations_ << "]";

    spinner.set_option(option::ForegroundColor{Color::green});
    spinner.set_option(option::ShowSpinner{false});
    spinner.set_option(option::ShowPercentage{false});
    spinner.set_option(option::ShowElapsedTime{false});
    spinner.set_option(option::ShowRemainingTime{false});
    spinner.set_option(option::PostfixText{os.str()});
    spinner.mark_as_completed();

    indicators::show_console_cursor(true);
  }
};

void register_function(const benchmark_config& config);
void execute_registered_functions();

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define BENCHMARK(Name)                                                \
  static_assert(true, Name " must be string literal");                 \
  namespace detail                                                     \
  {                                                                    \
  /* function we later define */                                       \
  static void CONCAT(_registered_fun_, __LINE__)();                    \
                                                                       \
  namespace /* ensure internal linkage for struct */                   \
  {                                                                    \
  /* helper struct for static registration in ctor */                  \
  struct CONCAT(_register_struct_, __LINE__)                           \
  {                                                                    \
      CONCAT(_register_struct_, __LINE__)()                            \
      { /* called once before main */                                  \
        register_function(benchmark_config {                           \
          .name = Name,                                                \
          .fn = CONCAT(_registered_fun_, __LINE__)});                  \
      }                                                                \
  } CONCAT(_register_struct_instance_, __LINE__);                      \
  }                                                                    \
  }                                                                    \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */ \
  void detail::CONCAT(_registered_fun_, __LINE__)()

static inline void signal_handler(int signal) {
  indicators::show_console_cursor(true);
  std::cout << termcolor::reset;
  exit(signal);
}

#define BENCHMARK_MAIN \
int main() { \
  std::signal(SIGTERM, signal_handler); \
  std::signal(SIGSEGV, signal_handler); \
  std::signal(SIGINT, signal_handler); \
  std::signal(SIGILL, signal_handler); \
  std::signal(SIGABRT, signal_handler); \
  std::signal(SIGFPE, signal_handler); \
  execute_registered_functions(); \
}