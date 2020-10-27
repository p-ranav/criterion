#include <benchmark/benchmark.hpp>
#include <set>

BENCHMARK("Insert 1K items into set<int>")
{
  std::set<int> set;
  for (auto i = 0; i < 1024; i++) {
    set.insert(i);
  }
}