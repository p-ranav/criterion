#include <criterion/details/benchmark_template_macro.hpp>
#include <criterion/details/benchmark_config.hpp>
#include <string>
#include <unordered_map>

std::unordered_multimap<std::string, benchmark_config> &
register_benchmark_template_data::registered_benchmark_templates() {
  static std::unordered_multimap<std::string, benchmark_config> m;
  return m;
}

void register_benchmark_template(const benchmark_config& config) {
  register_benchmark_template_data::registered_benchmark_templates().insert({config.name, config});
}