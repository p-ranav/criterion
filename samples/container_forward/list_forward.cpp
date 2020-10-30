#include <criterion/criterion.hpp>
#include <list>

BENCHMARK(ListIntForward)
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
REGISTER_BENCHMARK(ListIntForward, "")