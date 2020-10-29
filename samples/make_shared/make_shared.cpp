#include <memory>
#include <string>
#include <criterion/criterion.hpp>

struct Foo {
  std::string data_1{"1234567891012131415161718192021"};
  int data_2{5};
  float data_3{3.1415f};
};

std::shared_ptr<Foo> Create_Foo_With_New() { 
  return std::shared_ptr<Foo>(new Foo()); 
}

std::shared_ptr<Foo> Create_Foo_With_MakeShared() { 
  return std::make_shared<Foo>(); 
}

BENCHMARK_TEMPLATE(shared_ptr, std::function<std::shared_ptr<Foo>()>)
{
  SETUP_BENCHMARK(
    const auto args = BENCHMARK_ARGUMENTS;
    auto test_function = std::get<0>(args);
  )

  auto foo_ptr = test_function();

  TEARDOWN_BENCHMARK(
    foo_ptr.reset();
  )
}

RUN_BENCHMARK_TEMPLATE(shared_ptr, "/new", Create_Foo_With_New)
RUN_BENCHMARK_TEMPLATE(shared_ptr, "/make_shared", Create_Foo_With_MakeShared)

// Lamba
// RUN_BENCHMARK_TEMPLATE(shared_ptr, "/new", [] { return std::shared_ptr<Foo>(new Foo()); })
// RUN_BENCHMARK_TEMPLATE(shared_ptr, "/make_shared", [] { return std::make_shared<Foo>(); })