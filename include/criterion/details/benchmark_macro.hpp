#pragma once
#include <criterion/details/benchmark.hpp>
#include <criterion/details/benchmark_config.hpp>
#include <criterion/details/csv_writer.hpp>
#include <chrono>
#include <functional>
#include <string.h>

#if defined(_WIN32)
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

namespace criterion {

struct benchmark_registration_helper_struct {
  static std::vector<benchmark_config> &
  registered_benchmarks() {
    static std::vector<benchmark_config> v;
    return v;
  }

  static void register_benchmark(const benchmark_config& config) {
    registered_benchmarks().push_back(config);
  }

  static void execute_registered_benchmarks() {
    for (const auto& config : registered_benchmarks()) {
      benchmark{config}.run();
    }
  }
};

}

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define BENCHMARK(Name)                                                        \
  namespace detail {                                                           \
  /* struct with static function we later define */                            \
  struct CONCAT(__benchmark_function_wrapper__, __LINE__) {                    \
    static void CONCAT(_registered_fun_, __LINE__)(                            \
        std::chrono::steady_clock::time_point &,                               \
        std::optional<std::chrono::steady_clock::time_point> &, \
        void*);        \
  };                                                                           \
                                                                               \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(_register_struct_, __LINE__) {                                 \
    CONCAT(_register_struct_, __LINE__)() { /* called once before main */      \
      criterion::benchmark_registration_helper_struct::register_benchmark(criterion::benchmark_config{                                     \
          .name = #Name,                                                        \
          .fn = CONCAT(__benchmark_function_wrapper__,                         \
                       __LINE__)::CONCAT(_registered_fun_, __LINE__)});        \
    }                                                                          \
  } CONCAT(_register_struct_instance_, __LINE__);                              \
  }                                                                            \
  }                                                                            \
                                                                               \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */         \
  void detail::CONCAT(__benchmark_function_wrapper__, __LINE__)::CONCAT(       \
      _registered_fun_, __LINE__)(                                             \
      [[maybe_unused]] std::chrono::steady_clock::time_point &                 \
          __benchmark_start_timestamp,                                         \
      [[maybe_unused]] std::optional<std::chrono::steady_clock::time_point> &  \
          __benchmark_teardown_timestamp, \
      [[maybe_unused]] void * __benchmark_parameters) \

#define SETUP_BENCHMARK(...)                                                   \
  __VA_ARGS__                                                                  \
  __benchmark_start_timestamp =                                                \
      std::chrono::steady_clock::now(); // updated benchmark start timestamp

#define TEARDOWN_BENCHMARK(...)                                                \
  __benchmark_teardown_timestamp = std::chrono::steady_clock::now();           \
  __VA_ARGS__

static inline void signal_handler(int signal) {
  indicators::show_console_cursor(true);
  std::cout << termcolor::reset;
  exit(signal);
}

#define CRITERION_BENCHMARK_MAIN                                                         \
  int main() {                                                                 \
    std::signal(SIGTERM, signal_handler);                                      \
    std::signal(SIGSEGV, signal_handler);                                      \
    std::signal(SIGINT, signal_handler);                                       \
    std::signal(SIGILL, signal_handler);                                       \
    std::signal(SIGABRT, signal_handler);                                      \
    std::signal(SIGFPE, signal_handler);                                       \
    criterion::benchmark_registration_helper_struct::execute_registered_benchmarks();                                           \
    criterion::csv_writer::write_results("results.csv", criterion::benchmark::results); \
  }