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