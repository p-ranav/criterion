#include <memory>
#include <string>
#include <benchmark/registration.hpp>

BENCHMARK("shared_ptr/new")
{
  SETUP_BENCHMARK(
    struct Foo {
      std::string data_1{"1234567891012131415161718192021"};
      int data_2{5};
      float data_3{3.1415f};
    };
  )

  std::shared_ptr<Foo> foo_ptr = std::shared_ptr<Foo>(new Foo());

  TEARDOWN_BENCHMARK(
    foo_ptr.reset();
  )
}

BENCHMARK("shared_ptr/make_shared")
{
  SETUP_BENCHMARK(
    struct Foo {
      std::string data_1{"1234567891012131415161718192021"};
      int data_2{5};
      float data_3{3.1415f};
    };
  )

  std::shared_ptr<Foo> foo_ptr = std::make_shared<Foo>();

  TEARDOWN_BENCHMARK(
    foo_ptr.reset();
  )
}