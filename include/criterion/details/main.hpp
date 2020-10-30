#pragma once
#include <criterion/details/indicators.hpp>
#include <criterion/details/macros.hpp>

static inline void signal_handler(int signal) {
  indicators::show_console_cursor(true);
  std::cout << termcolor::reset;
  exit(signal);
}

#define CRITERION_BENCHMARK_MAIN                                                                   \
  int main() {                                                                                     \
    std::signal(SIGTERM, signal_handler);                                                          \
    std::signal(SIGSEGV, signal_handler);                                                          \
    std::signal(SIGINT, signal_handler);                                                           \
    std::signal(SIGILL, signal_handler);                                                           \
    std::signal(SIGABRT, signal_handler);                                                          \
    std::signal(SIGFPE, signal_handler);                                                           \
    criterion::benchmark_registration_helper_struct::execute_registered_benchmarks();              \
    criterion::csv_writer::write_results("results.csv", criterion::benchmark::results);            \
  }