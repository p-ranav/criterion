#include <vector>
#include <benchmark/benchmark.hpp>

std::vector<int> sorted(const std::vector<int>& vec) {
  auto vec_copy = vec;
  std::sort(vec_copy.begin(), vec_copy.end());
  return vec_copy;
}

std::vector<int> vec1{5, 4, 3, 2, 1};
std::vector<int> vec2(10, 0);
std::vector<int> vec3(100, 0);
std::vector<int> vec4(1000, 0);

BENCHMARK("Vector sort #1")
{
  auto result = sorted(vec1);
}

BENCHMARK("Vector sort #2")
{
  auto result = sorted(vec2); 
}

BENCHMARK("Vector sort #3")
{
  auto result = sorted(vec3); 
}

BENCHMARK("Vector sort #4")
{
  auto result = sorted(vec4);
}