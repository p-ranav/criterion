#pragma once
#include <chrono>
#include <functional>
#include <optional>
#include <string>

struct benchmark_config {
  std::string name;
  using Fn = std::function<void(std::chrono::steady_clock::time_point&, // start time stamp
                                std::optional<std::chrono::steady_clock::time_point>&)>; // teardown time stamp
  Fn fn;
};