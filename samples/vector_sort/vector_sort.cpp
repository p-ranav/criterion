#include <vector>
#include <benchmark/registration.hpp>

std::vector<int> sorted(const std::vector<int>& vec) {
  auto vec_copy = vec;
  std::sort(vec_copy.begin(), vec_copy.end());
  return vec_copy;
}

BENCHMARK("vector_sort/10")
{
  SETUP_BENCHMARK(
    static std::vector<int> vec(10, 0);
  )

  auto result = sorted(vec); 
}

BENCHMARK("vector_sort/100")
{
  SETUP_BENCHMARK(
    static std::vector<int> vec(100, 0);
  )

  auto result = sorted(vec); 
}

BENCHMARK("vector_sort/1000")
{
  SETUP_BENCHMARK(
    static std::vector<int> vec(1000, 0);
  )

  auto result = sorted(vec); 
}