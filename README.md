<p align="center">
  <img height="90" src="img/logo.png"/>  
</p> 

## Highlights

`Criterion` is a micro-benchmarking library for modern C++.

* Convenient static registration macros for setting up benchmarks
* Parameterized benchmarks (e.g., vary input size)
* Statistical analysis across multiple runs
* Requires compiler support for `C++17` or newer standard
* Header-only library - single header file version available at `single_include/`
* MIT License

## Table of Contents

*    [Getting Started](#getting-started)
     *    [Simple Benchmark](#simple-benchmark)
     *    [Passing Arguments](#passing-arguments)
     *    [Passing Arguments (Part 2)](#passing-arguments-part-2)
*    [Building Library and Samples](#building-library-and-samples)
*    [Generating Single Header](#generating-single-header)
*    [Contributing](#contributing)
*    [License](#license)

## Getting Started

Let's say we have this merge sort implementation that needs to be benchmarked.

```cpp
template<typename RandomAccessIterator, typename Compare>
void merge_sort(RandomAccessIterator first, RandomAccessIterator last,
                Compare compare, std::size_t size) {
  if (size < 2) return;
  auto middle = first + size / 2;
  merge_sort(first, middle, compare, size / 2);
  merge_sort(middle, last, compare, size - size/2);
  std::inplace_merge(first, middle, last, compare);
}
```

### Simple Benchmark

Include `<criterion/criterion.hpp>` and you're good to go.

* Use the `BENCHMARK` macro to declare a benchmark
* Use `SETUP_BENCHMARK` and `TEARDOWN_BENCHMARK` to perform setup and teardown tasks
  - These tasks are not part of the measurement

```cpp
#include <criterion/criterion.hpp>

BENCHMARK(MergeSort)
{
  SETUP_BENCHMARK(
    const auto size = 100;
    std::vector<int> vec(size, 0);
  )
 
  // Code to be benchmarked
  merge_sort(vec.begin(), vec.end(), std::less<int>(), size);
  
  TEARDOWN_BENCHMARK(
    vec.clear();
  )
}

CRITERION_BENCHMARK_MAIN()
```

```console
foo@bar:~$ ./build/samples/merge_sort/merge_sort
✓ MergeSort/100 7.68us ± 0.0727% (7.54us … 117us)
```

The above benchmark measures performance of our merge sort implementation on a vector of size 100. What if we want to run this benchmark on a variety of sizes?

### Passing Arguments

* The `BENCHMARK` macro can take typed parameters
* Use `GET_ARGUMENTS(n)` to get the nth argument passed to the benchmark
* For benchmarks that require arguments, use `INVOKE_BENCHMARK_FOR_EACH` and provide arguments

```cpp
#include <criterion/criterion.hpp>

BENCHMARK(MergeSort, std::size_t) // <- one parameter to be passed to the benchmark
{
  SETUP_BENCHMARK(
    const auto size = GET_ARGUMENT(0); // <- get the argument passed to the benchmark
    std::vector<int> vec(size, 0);
  )
 
  // Code to be benchmarked
  merge_sort(vec.begin(), vec.end(), std::less<int>(), size);
  
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

CRITERION_BENCHMARK_MAIN()
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

### Passing Arguments (Part 2)

`Criterion` allows for passing arguments like `std::function`. This allows for benchmarking and comparison of implementations. 

Let's say we have the following struct and we need to create a `std::shared_ptr` to it.

```cpp
struct Song {
  std::string artist;
  std::string title;
  Song(const std::string& artist_, const std::string& title_) :
    artist{ artist_ }, title{ title_ } {}
};
```

Here are two implementations for constructing the `std::shared_ptr`:

```cpp
// Functions to be tested
auto Create_With_New() { 
  return std::shared_ptr<Song>(new Song("Black Sabbath", "Paranoid")); 
}

auto Create_With_MakeShared() { 
  return std::make_shared<Song>("Black Sabbath", "Paranoid"); 
}
```

We can setup a single benchmark that takes a `std::function<>` and measures performance like below.

```cpp
BENCHMARK(ConstructSharedPtr, std::function<std::shared_ptr<Song>()>) 
{
  SETUP_BENCHMARK(
    auto test_function = GET_ARGUMENT(0);
  )

  // Code to be benchmarked
  auto song_ptr = test_function();
}

INVOKE_BENCHMARK_FOR_EACH(ConstructSharedPtr, 
  ("/new", Create_With_New),
  ("/make_shared", Create_With_MakeShared)
)

CRITERION_BENCHMARK_MAIN()
```

```console
foo@bar:~$ ./build/samples/make_shared/make_shared
✓ ConstructSharedPtr/new 184ns ± 0.163% (137ns … 69.1us)
✓ ConstructSharedPtr/make_shared 68.8ns ± 0.581% (67ns … 66.7us)
```

The above benchmark shows that using `std::make_shared` is the way to go.

## Building Library and Samples

```bash
cmake -Hall -Bbuild
cmake --build build

# run `merge_sort` sample
./build/samples/merge_sort/merge_sort
```

## Generating Single Header

```bash
python3 utils/amalgamate/amalgamate.py -c single_include.json -s .
```

## Contributing
Contributions are welcome, have a look at the [CONTRIBUTING.md](CONTRIBUTING.md) document for more information.

## License
The project is available under the [MIT](https://opensource.org/licenses/MIT) license.
