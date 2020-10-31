#include <criterion/criterion.hpp>

template<typename RandomAccessIterator, typename Compare>
void mergeSort(RandomAccessIterator first, RandomAccessIterator last,
               Compare compare, std::size_t size) {
  if (size < 2) return;
  auto middle = first + size / 2;
  mergeSort(first, middle, compare, size / 2);
  mergeSort(middle, last, compare, size - size/2);
  std::inplace_merge(first, middle, last, compare);
}

BENCHMARK(MergeSort, std::size_t)
{
  SETUP_BENCHMARK(
    const auto size = GET_ARGUMENT(0);
    std::vector<int> vec(size, 0);
  )

  mergeSort(vec.begin(), vec.end(), std::less<int>(), size);
}

REGISTER_BENCHMARK_FOR_EACH(MergeSort,
  ("/10", 10),
  ("/100", 100),
  ("/1000", 1000),
  ("/10000", 10000),
  ("/100000", 100000),
  ("/1000000", 1000000),
  ("/10000000", 10000000),
  ("/100000000", 100000000)
)