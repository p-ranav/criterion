#include <sstream>
#include <benchmark/benchmark.hpp>

BENCHMARK("Sstream Hello World")
{
  std::vector<std::string> results;

  for (size_t i = 0; i < 100; i++) {
    std::stringstream os;
    os << "Hello World " << i << "\n";
    results.push_back(os.str());
  }
}