<p align="center">
  <img height="90" src="img/logo.png"/>  
</p>

`Criterion` is a micro-benchmarking library for modern C++. 

## Highlights

* Convenient static registration macros for setting up benchmarks
* Parameterized benchmarks (e.g., vary the vector size)
* Statistical analysis across multiple runs
* Requires compiler support for `C++17` or newer standard
* Header-only library - single header file version available at `single_include/`
* MIT License

## Quick Start

Let's say we have this merge sort implementation that needs to be benchmarked.

```cpp
template<typename RandomAccessIterator, typename Compare>
void mergeSort(RandomAccessIterator first, RandomAccessIterator last,
               Compare compare, std::size_t size) {
  if (size < 2) return;
  auto middle = first + size / 2;
  mergeSort(first, middle, compare, size / 2);
  mergeSort(middle, last, compare, size - size/2);
  std::inplace_merge(first, middle, last, compare);
}
```

Simply include `criterion/criterion.hpp` and you're good to go.

* Use the `BENCHMARK` macro to declare a benchmark
  - Benchmarks can take zero or more typed arguments
* Use `GET_ARGUMENTS(n)` to get the nth parameter passed to the benchmark
* Use `SETUP_BENCHMARK` and `TEARDOWN_BENCHMARK` to perform setup and teardown tasks
  - These tasks are not part of the measurement
* For benchmarks are require arguments, use `INVOKE_BENCHMARK_FOR_EACH` and provide arguments

```cpp
#include <criterion/criterion.hpp>

BENCHMARK(MergeSort, std::size_t) // <- one parameter to be passed to the benchmark
{
  SETUP_BENCHMARK(
    const auto size = GET_ARGUMENT(0); // <- get the argument passed to the benchmark
    std::vector<int> vec(size, 0);
  )
 
  // Code to be benchmarked
  mergeSort(vec.begin(), vec.end(), std::less<int>(), size);
  
  TEARDOWN_BENCHMARK(
    vec.clear();
  )
}

// Run the above benchmark for a number of inputs:

INVOKE_BENCHMARK_FOR_EACH(MergeSort,
  ("/10", 10),
  ("/100", 100),
  ("/1K", 1000),
  ("/10K", 10000),
  ("/100K", 100000),
  ("/1M", 1000000),
  ("/10M", 10000000),
  ("/100M", 100000000)
)

CRITERION_BENCHMARK_MAIN
```

Sample console output:

```console
foo@bar:~$ ./build/samples/merge_sort/merge_sort
✓ MergeSort/10 677ns ± 0.0724% (646ns … 68.9us)
✓ MergeSort/100 7.21us ± 0.0426% (7.1us … 228us)
✓ MergeSort/1K 70us ± 0.0349% (69.9us … 1.85ms)
✓ MergeSort/10K 743us ± 0.0394% (727us … 1.87ms)
✓ MergeSort/100K 8.07ms ± 0.958% (7.36ms … 13.2ms)
✓ MergeSort/1M 78.2ms ± 2.06% (76.6ms … 99.8ms)
✓ MergeSort/10M 891ms ± 3.54% (847ms … 1.02s)
✓ MergeSort/100M 9.52s ± 3.76% (9s … 10.1s)
```
