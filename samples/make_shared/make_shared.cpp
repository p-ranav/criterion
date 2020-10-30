#include <memory>
#include <string>
#include <criterion/criterion.hpp>

struct Foo {
  std::string data_1{"1234567891012131415161718192021"};
  int data_2{5};
  float data_3{3.1415f};
};

BENCHMARK(ConstructSharedPtr, std::function<std::shared_ptr<Foo>()>)
{
  SETUP_BENCHMARK(
    auto test_function = GET_ARGUMENT(0);
  )

  auto foo_ptr = test_function();

  TEARDOWN_BENCHMARK(
    foo_ptr.reset();
  )
}

// Functions to be tested
auto Create_Foo_With_New() { return std::shared_ptr<Foo>(new Foo()); }
auto Create_Foo_With_MakeShared() { return std::make_shared<Foo>(); }

REGISTER_BENCHMARK_FOR_EACH(ConstructSharedPtr,
  ("/new", Create_Foo_With_New),
  ("/make_shared", Create_Foo_With_MakeShared)
)