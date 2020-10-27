#include <benchmark/benchmark.hpp>
#include <string>
#include <vector>

BENCHMARK("Allocate vector of ints of size 1E7")
{
  const auto large = std::vector<int>(1E7, 0);
}

BENCHMARK("Allocate vector of strings of size 1E5")
{
  const auto large = std::vector<std::string>(1E5, "");
}