#include <criterion/criterion.hpp>

BENCHMARK_TEMPLATE(Memcpy, size_t)
{
  SETUP_BENCHMARK(
    const auto size = BENCHMARK_ARGUMENTS(0);
    char* src = new char[size];
    char* dst = new char[size];
    memset(src, 'x', size);
  ) 

  memcpy(dst, src, size);

  TEARDOWN_BENCHMARK(
    delete[] src;
    delete[] dst;
  )
}

RUN_BENCHMARK_TEMPLATE(Memcpy, "/32k", 32000)
RUN_BENCHMARK_TEMPLATE(Memcpy, "/128k", 128000)
RUN_BENCHMARK_TEMPLATE(Memcpy, "/1M", 1000000)