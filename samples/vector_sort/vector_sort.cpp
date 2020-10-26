#include <array>
#include <vector>
#include <benchmark/benchmark.hpp>

int main() {

  auto sorted = [](auto& vec) {
    auto vec_copy = vec;
    std::sort(vec_copy.begin(), vec_copy.end());
    return vec_copy;
  };

  {
    std::vector<int> vec{5, 4, 3, 2, 1};
    benchmark("Vector sort #1", [&vec, &sorted] {
      return sorted(vec);
    });
  }

  {
    std::vector<int> vec(100, 2);
    benchmark("Vector sort #2", [&vec, &sorted] {
      return sorted(vec);
    });
  }

  {
    std::vector<int> vec(1000, 0);
    benchmark("Vector sort #3", [&vec, &sorted] {
      return sorted(vec);
    });
  }
}