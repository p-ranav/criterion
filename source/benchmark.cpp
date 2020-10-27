#include <benchmark/benchmark.hpp>

static std::vector<std::pair<std::string, benchmark_function>> &
registered_functions() {
  static std::vector<std::pair<std::string, benchmark_function>> v;
  return v;
}

void register_function(const std::string &name, benchmark_function function) {
  registered_functions().push_back(std::make_pair(name, function));
}

void execute_registered_functions() {
  for (auto& name_fn_pair : registered_functions()) {
    benchmark(name_fn_pair.first, name_fn_pair.second);
  }
}