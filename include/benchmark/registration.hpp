#pragma once
#include <benchmark/benchmark.hpp>

void register_function(const benchmark_config& config);
void execute_registered_functions();

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define BENCHMARK(Name)                                                \
  static_assert(true, Name " must be string literal");                 \
  namespace detail                                                     \
  {                                                                    \
  /* function we later define */                                       \
  static void CONCAT(_registered_fun_, __LINE__)(long double&);                    \
  static long double CONCAT(CONCAT(_registered_fun_, __LINE__), _setup_duration) = 0; \
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
          .fn = CONCAT(_registered_fun_, __LINE__),                  \
          .setup_duration = CONCAT(CONCAT(_registered_fun_, __LINE__), _setup_duration)}); \
      }                                                                \
  } CONCAT(_register_struct_instance_, __LINE__);                      \
  }                                                                    \
  }                                                                    \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */ \
  void detail::CONCAT(_registered_fun_, __LINE__)(                     \
    [[maybe_unused]]long double & __benchmark_setup_duration) \

#define SETUP_BENCHMARK(...) \
  const auto __benchmark_setup_start = std::chrono::steady_clock::now(); \
  __VA_ARGS__ \
  const auto __benchmark_setup_end = std::chrono::steady_clock::now(); \
  __benchmark_setup_duration = static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(__benchmark_setup_end - __benchmark_setup_start).count());

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