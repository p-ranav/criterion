#include <criterion/criterion.hpp>
#include <thread>

template<typename RandomAccessIterator, typename Compare>
void merge_sort(RandomAccessIterator first, RandomAccessIterator last,
                Compare compare, std::size_t size) {
  if (size < 2) return;
  auto middle = first + size / 2;
  merge_sort(first, middle, compare, size / 2);
  merge_sort(middle, last, compare, size - size/2);
  std::inplace_merge(first, middle, last, compare);
}

BENCHMARK(MergeSort, std::size_t)
{
  SETUP_BENCHMARK(
    const auto size = GET_ARGUMENT(0);
    std::vector<int> vec(size, 0);
  )

  merge_sort(vec.begin(), vec.end(), std::less<int>(), size);
}

INVOKE_BENCHMARK_FOR_EACH(MergeSort,
  ("/10", 10),
  ("/100", 100),
  ("/1K", 1000),
  ("/10K", 10000),
  ("/100K", 100000),
  ("/1M", 1000000),
  ("/10M", 10000000)
)

BENCHMARK(MergeSortMultithreaded, std::size_t)
{
  SETUP_BENCHMARK(
    const auto num_threads = GET_ARGUMENT(0);
    std::vector<int> vec(10000, 0);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);
  )

  for (std::size_t i = 0; i < num_threads; i++) {
    threads.push_back(
      std::thread(
        &merge_sort<std::vector<int>::iterator, std::less<int>>, 
          vec.begin(), vec.end(), std::less<int>(), 10000
      )
    );
  }

  TEARDOWN_BENCHMARK(
    for (auto& t: threads) {
      t.join();
    }
  )
}

// Runs merge sort to sort a vector of numbers on multiple threads
INVOKE_BENCHMARK_FOR_EACH(MergeSortMultithreaded,
  ("/10000/1", 1),
  ("/10000/2", 2),
  ("/10000/4", 4),
  ("/10000/8", 8),
  ("/10000/16", 16),
  ("/10000/32", 32)
)