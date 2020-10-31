#include <criterion/criterion.hpp>

BENCHMARK(Memcpy, size_t) 
{
  SETUP_BENCHMARK(
    const auto size = GET_ARGUMENT(0); 
    char *src = new char[size];
    char *dst = new char[size]; 
    memset(src, 'x', size);
  )

  // Code to be benchmarked
  memcpy(dst, src, size);

  TEARDOWN_BENCHMARK(
    delete[] src; 
    delete[] dst;
  )
}

INVOKE_BENCHMARK_FOR_EACH(Memcpy, 
  ("/32k", 32000), 
  ("/128k", 128000), 
  ("/1M", 1000000)
)