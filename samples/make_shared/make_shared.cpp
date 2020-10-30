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

BENCHMARK(ConstructSharedPtr, std::function<std::shared_ptr<Foo>()>)
{
  SETUP_BENCHMARK(
    auto test_function = BENCHMARK_ARGUMENTS(0);
  )

  auto foo_ptr = test_function();

  TEARDOWN_BENCHMARK(
    foo_ptr.reset();
  )
}

REGISTER_BENCHMARK(ConstructSharedPtr, "/new", Create_Foo_With_New)
REGISTER_BENCHMARK(ConstructSharedPtr, "/make_shared", Create_Foo_With_MakeShared)

// Lamba
// REGISTER_BENCHMARK(shared_ptr, "/new", [] { return std::shared_ptr<Foo>(new Foo()); })
// REGISTER_BENCHMARK(shared_ptr, "/make_shared", [] { return std::make_shared<Foo>(); })