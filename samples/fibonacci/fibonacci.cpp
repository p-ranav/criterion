#include <criterion/criterion.hpp>

uint64_t Fibonacci(uint64_t n) {
  if (n <= 1) 
    return 1;
  else
    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

BENCHMARK(Fibonacci, uint64_t)
{
  SETUP_BENCHMARK(
    const auto input = GET_ARGUMENT(0);
  )

  [[maybe_unused]] const auto result = Fibonacci(input);
}

REGISTER_BENCHMARK_FOR_EACH(Fibonacci,
  ("/19", 19),
  ("/20", 20),
  ("/21", 21)
)

// Example output:
//
// Fibonacci/19
//    μ = 20.9us ± 0.0433% [N = 390030]
// Fibonacci/20
//    μ = 33.8us ± 0.0355% [N = 295370]
// Fibonacci/21
//    μ = 54.6us ± 0.0304% [N = 182650]