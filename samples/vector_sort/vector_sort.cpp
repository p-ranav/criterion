#include <vector>
#include <benchmark/benchmark.hpp>

std::vector<int> sorted(const std::vector<int>& vec) {
  auto vec_copy = vec;
  std::sort(vec_copy.begin(), vec_copy.end());
  return vec_copy;
}

BENCHMARK("Vector sort #1")
{
  std::vector<int> vec{5, 4, 3, 2, 1};
  auto result = sorted(vec);
}

BENCHMARK("Vector sort #2")
{
  std::vector<int> vec(10, 0);
  auto result = sorted(vec); 
}

BENCHMARK("Vector sort #3")
{
  std::vector<int> vec(100, 0);
  auto result = sorted(vec); 
}

BENCHMARK("Vector sort #4")
{
  std::vector<int> vec(1000, 0);
  auto result = sorted(vec);
}