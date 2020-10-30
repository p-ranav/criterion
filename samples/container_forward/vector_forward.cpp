#include <criterion/criterion.hpp>
#include <vector>

BENCHMARK(VectorIntForward)
{
  SETUP_BENCHMARK(
    std::vector<int> vec;
    for (int i = 0; i < 1000000; ++i)
      vec.push_back(rand());
  )

  // Code being benchmarked
  for (auto it = vec.begin(); it != vec.end(); ++it)
    ++(*it);

  TEARDOWN_BENCHMARK(
    vec.clear();
  )
}