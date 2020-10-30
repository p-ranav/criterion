#include <vector>
#include <criterion/criterion.hpp>

BENCHMARK_TEMPLATE(VectorSort, size_t)
{
  SETUP_BENCHMARK(
    const auto size = BENCHMARK_ARGUMENTS(0);
    std::vector<int> vec(size, 0);
  )

  // Code being benchmarked
  std::sort(vec.begin(), vec.end());

  TEARDOWN_BENCHMARK(
    vec.clear();
  )
}

RUN_BENCHMARK_TEMPLATE(VectorSort, "/10", 10)
RUN_BENCHMARK_TEMPLATE(VectorSort, "/100", 100)
RUN_BENCHMARK_TEMPLATE(VectorSort, "/1000", 1000)
RUN_BENCHMARK_TEMPLATE(VectorSort, "/10000", 10000)

// vector_sort/10
//    μ = 7.7ns ± 3.9% [N = 100000000]
// vector_sort/100
//    μ = 94.2ns ± 2.25% [N = 100000000]
// vector_sort/1000
//    μ = 681ns ± 0.825% [N = 35419120]
// vector_sort/10000
//    μ = 6.58us ± 0.152% [N = 6131200]