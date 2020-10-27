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

// Example output on Macbook Pro 2019
// ✔ sstream_hello_world.cpp:4 [Sstream Hello World] [00m:54s<00m:00s] μ = 24.8us ± 0.164%