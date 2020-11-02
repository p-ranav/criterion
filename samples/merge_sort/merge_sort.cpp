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
  ("/100K", 100000)
)