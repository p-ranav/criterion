#include <vector>
#include <benchmark/benchmark.hpp>

std::vector<int> sorted(const std::vector<int>& vec) {
  auto vec_copy = vec;
  std::sort(vec_copy.begin(), vec_copy.end());
  return vec_copy;
}

// std::vector<int> vec10(10, 0);
std::vector<int> vec100(100, 0);
std::vector<int> vec1000(1000, 0);

BENCHMARK("vector_sort/10")
{

  SETUP_BENCHMARK(
    std::vector<int> vec10(10, 0);
  )

  auto result = sorted(vec10); 
}

BENCHMARK("vector_sort/100")
{
  auto result = sorted(vec100); 
}

BENCHMARK("vector_sort/1000")
{
  auto result = sorted(vec1000);
}