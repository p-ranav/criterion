#include <benchmark/benchmark.hpp>
#include <algorithm>
#include <vector>

int main() {
  std::vector<size_t> sizes = {10, 20, 30, 40, 50};

  std::for_each(sizes.begin(), sizes.end(), 
    [](size_t &n) { 
      const auto name = std::string{"vector_construct_sort/"} + std::to_string(n);
      benchmark(name, 
      [&n] {
        auto vec = std::vector<int>(n, 0);
        std::sort(vec.begin(), vec.end());
      });
    }
  );

}