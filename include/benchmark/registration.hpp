#pragma once
#include <benchmark/benchmark.hpp>
#include <benchmark/benchmark_config.hpp>
#include <chrono>
#include <string.h>

#if defined(_WIN32)
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

void register_benchmark(const benchmark_config &config);
void execute_registered_benchmarks();

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define BENCHMARK(Name)                                                        \
  static_assert(true, Name " must be string literal");                         \
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
      register_benchmark(benchmark_config{                                     \
          .name = Name,                                                        \
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

#define BENCHMARK_MAIN                                                         \
  int main() {                                                                 \
    std::signal(SIGTERM, signal_handler);                                      \
    std::signal(SIGSEGV, signal_handler);                                      \
    std::signal(SIGINT, signal_handler);                                       \
    std::signal(SIGILL, signal_handler);                                       \
    std::signal(SIGABRT, signal_handler);                                      \
    std::signal(SIGFPE, signal_handler);                                       \
    execute_registered_benchmarks();                                           \
  }