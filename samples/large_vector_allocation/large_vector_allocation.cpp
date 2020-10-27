#include <benchmark/benchmark.hpp>
#include <string>
#include <vector>

BENCHMARK("vector_construct/int/1E7")
{
  const auto large = std::vector<int>(1E7, 0);
}

BENCHMARK("vector_construct/string/1E5")
{
  const auto large = std::vector<std::string>(1E5, "");
}