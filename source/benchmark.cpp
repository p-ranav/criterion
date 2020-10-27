#include <benchmark/benchmark.hpp>

static std::vector<benchmark_config> &
registered_functions() {
  static std::vector<benchmark_config> v;
  return v;
}

void register_function(const benchmark_config& config) {
  registered_functions().push_back(config);
}

void execute_registered_functions() {
  for (const auto& config : registered_functions()) {
    benchmark(config.name, config.fn);
  }
}