#include <criterion/criterion.hpp>
#include <list>
#include <vector>

BENCHMARK("std::list<int>.forward")
{
  SETUP_BENCHMARK(
    std::list<int> list;
    for (int i = 0; i < 1000000; ++i)
      list.push_back(rand());
  )

  // Code being benchmarked
  for (auto it = list.begin(); it != list.end(); ++it)
    ++(*it);

  TEARDOWN_BENCHMARK(
    list.clear();
  )
}

BENCHMARK("std::vector<int>.forward")
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