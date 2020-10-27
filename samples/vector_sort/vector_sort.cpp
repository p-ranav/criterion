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

// Example output on Macbook Pro 15 2019
// ✔ vector_sort.cpp:15 [Vector sort #1] [02m:22s<00m:00s] μ = 64.8ns ± 4.99%
// ✔ vector_sort.cpp:20 [Vector sort #2] [02m:16s<00m:00s] μ = 85.1ns ± 4.49%
// ✔ vector_sort.cpp:25 [Vector sort #3] [02m:34s<00m:00s] μ = 229ns ± 3.13%
// ✔ vector_sort.cpp:30 [Vector sort #4] [01m:11s<00m:00s] μ = 930ns ± 1.55%