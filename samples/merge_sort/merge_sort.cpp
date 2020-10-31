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

template<
    typename RandomAccessIterator,
    typename Compare = std::less<>
>
void mergeSortImpl(RandomAccessIterator first, RandomAccessIterator last,
               Compare compare={})
{
  std::size_t size = last - first;
  mergeSortImpl(first, last, compare, size);
}

BENCHMARK(MergeSort, std::size_t)
{
  SETUP_BENCHMARK(
    const auto size = GET_ARGUMENT(0);
    std::vector<int> vec(size, 0);
    // vec.reserve(size);
    // for (size_t i = 0; i < size; i++) {
    //   vec.push_back(rand());
    // }
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