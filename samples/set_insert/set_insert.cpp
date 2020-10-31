#include <criterion/criterion.hpp>
#include <set>

BENCHMARK(SetInsert) 
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