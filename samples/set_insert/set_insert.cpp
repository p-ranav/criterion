#include <benchmark/criterion.hpp>
#include <set>
#include <unordered_set>

BENCHMARK("set_insert")
{
  SETUP_BENCHMARK(
    std::set<int> set;
    for (auto i = 0; i < 1E7; i++)
      set.insert(i);
  )

  set.insert(654321);

  TEARDOWN_BENCHMARK(
    set.clear();
  )
}

BENCHMARK("unordered_set_insert")
{
  SETUP_BENCHMARK(
    std::unordered_set<int> set;
    for (auto i = 0; i < 1E7; i++)
      set.insert(i);
  )

  set.insert(654321);

  TEARDOWN_BENCHMARK(
    set.clear();
  )
}