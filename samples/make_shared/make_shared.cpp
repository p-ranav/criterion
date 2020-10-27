#include <memory>
#include <string>
#include <benchmark/benchmark.hpp>

struct Foo {
  std::string data_1{"1234567891012131415161718192021"};
  int data_2{5};
  float data_3{3.1415f};
};

BENCHMARK("Create shared_ptr with new")
{
  std::shared_ptr<Foo> foo_ptr = std::shared_ptr<Foo>(new Foo());
}

BENCHMARK("Create shared_ptr with make_shared")
{
  std::shared_ptr<Foo> foo_ptr = std::make_shared<Foo>();
}

// Example output on Macbook Pro 15 2019
// ✔ make_shared.cpp:11 [Create shared_ptr with new] [02m:43s<00m:00s] μ = 248ns ± 1.58%
// ✔ make_shared.cpp:16 [Create shared_ptr with make_shared] [02m:30s<00m:00s] μ = 140ns ± 2.07%