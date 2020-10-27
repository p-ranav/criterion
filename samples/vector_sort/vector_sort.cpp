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
// ✔ vector_sort.cpp:15 [Vector sort #1] [02m:06s<00m:00s] μ = 110ns ± 0.679%
// ✔ vector_sort.cpp:20 [Vector sort #2] [02m:46s<00m:00s] μ = 201ns ± 1.46%
// ✔ vector_sort.cpp:25 [Vector sort #3] [02m:48s<00m:00s] μ = 244ns ± 1.34%
// ✔ vector_sort.cpp:30 [Vector sort #4] [01m:48s<00m:00s] μ = 1.25us ± 0.47%