#include <criterion/criterion.hpp>
#include <unordered_set>

BENCHMARK(UnorderedSetInsert) 
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