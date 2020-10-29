#include <benchmark/registration.hpp>
#include <benchmark/benchmark_config.hpp>
#include <functional>

static std::vector<benchmark_config> &
registered_benchmarks() {
  static std::vector<benchmark_config> v;
  return v;
}

void register_benchmark(const benchmark_config& config) {
  registered_benchmarks().push_back(config);
}

void execute_registered_benchmarks() {
  for (const auto& config : registered_benchmarks()) {
    benchmark{config}.run();
  }
}