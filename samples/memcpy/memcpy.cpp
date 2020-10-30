#include <criterion/criterion.hpp>

BENCHMARK_TEMPLATE(Memcpy, size_t)
{
  SETUP_BENCHMARK(
    const auto size = BENCHMARK_ARGUMENTS(0);
    int * src = new int[size];
    std::memset(src, 0, size);
    int * dst = new int[size];
  ) 

  memcpy(dst, src, sizeof(int) * size);

  TEARDOWN_BENCHMARK(
    delete[] src;
    delete[] dst;
  )
}

RUN_BENCHMARK_TEMPLATE(Memcpy, "/int/32", 32)
RUN_BENCHMARK_TEMPLATE(Memcpy, "/int/32k", 32000)
RUN_BENCHMARK_TEMPLATE(Memcpy, "/int/128k", 128000)
RUN_BENCHMARK_TEMPLATE(Memcpy, "/int/1M", 1000000)