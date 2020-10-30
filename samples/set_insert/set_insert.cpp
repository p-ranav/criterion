#include <criterion/criterion.hpp>
#include <set>
#include <unordered_set>

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
REGISTER_BENCHMARK(SetInsert, "/insert_one")

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
REGISTER_BENCHMARK(UnorderedSetInsert, "/unordered/insert_one")