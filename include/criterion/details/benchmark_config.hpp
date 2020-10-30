#pragma once
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

namespace criterion {

struct benchmark_config {
  static inline std::tuple<> empty_tuple{};
  std::string name;
  using Fn = std::function<void(
      std::chrono::steady_clock::time_point &,                // start time stamp
      std::optional<std::chrono::steady_clock::time_point> &, // teardown time stamp
      void *parameters)>;                                     // benchmark parameters
  Fn fn;
  std::string parameterized_instance_name = "";
  void *parameters = (void *)(&empty_tuple);

  enum class benchmark_reporting_type { console };
  benchmark_reporting_type reporting_type = benchmark_reporting_type::console;
};

} // namespace criterion