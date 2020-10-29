#include <vector>
#include <benchmark/registration.hpp>
#include <benchmark/benchmark_template.hpp>

BENCHMARK_TEMPLATE(vector_sort, size_t)
{
  SETUP_BENCHMARK(
    const auto params = BENCHMARK_ARGUMENTS;
    std::vector<int> vec(std::get<0>(params), 0);
  )

  // Code being benchmarked
  std::sort(vec.begin(), vec.end());

  TEARDOWN_BENCHMARK(
    vec.clear();
  )
}

RUN_BENCHMARK_TEMPLATE(vector_sort, "/10", 10)
RUN_BENCHMARK_TEMPLATE(vector_sort, "/100", 100)
RUN_BENCHMARK_TEMPLATE(vector_sort, "/1000", 1000)
RUN_BENCHMARK_TEMPLATE(vector_sort, "/10000", 10000)

// vector_sort/10
//    μ = 7.7ns ± 3.9% [N = 100000000]
// vector_sort/100
//    μ = 94.2ns ± 2.25% [N = 100000000]
// vector_sort/1000
//    μ = 681ns ± 0.825% [N = 35419120]
// vector_sort/10000
//    μ = 6.58us ± 0.152% [N = 6131200]