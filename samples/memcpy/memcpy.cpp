#include <criterion/criterion.hpp>

BENCHMARK(Memcpy, size_t)
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

REGISTER_BENCHMARK(Memcpy, "/32k", 32000)
REGISTER_BENCHMARK(Memcpy, "/128k", 128000)
REGISTER_BENCHMARK(Memcpy, "/1M", 1000000)