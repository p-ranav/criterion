#include <vector>
#include <benchmark/registration.hpp>

BENCHMARK("vector_sort/11")
{
  SETUP_BENCHMARK(
    std::vector<int> vec;
    for (auto& i : {5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5})
      vec.push_back(i);
  )

  // Code being benchmarked
  std::sort(vec.begin(), vec.end());

  TEARDOWN_BENCHMARK(
    vec.clear();
  )
}