#include <functional>
#include <string>
#include <vector>
#include <criterion/criterion.hpp>

auto IntToStringConversionTest(int count)
{
  using namespace std;
  vector<int> inputNumbers(count);
  vector<string> outNumbers;

  iota(begin(inputNumbers), end(inputNumbers), 0);
  for (auto &num : inputNumbers)
      outNumbers.push_back(to_string(num));

  return outNumbers;
}

auto DoubleToStringConversionTest(int count)
{
  using namespace std;
  vector<double> inputNumbers(count);
  vector<string> outNumbers;

  iota(begin(inputNumbers), end(inputNumbers), 0.12345);
  for (auto &num : inputNumbers)
      outNumbers.push_back(to_string(num));

  return outNumbers;
}

BENCHMARK(ToString, /* parameters -> */ std::function<std::vector<std::string>(int)>, int)
{
  SETUP_BENCHMARK(
    const auto fn = BENCHMARK_ARGUMENTS(0);
    const auto size = BENCHMARK_ARGUMENTS(1);
  )
  fn(size);
  TEARDOWN_BENCHMARK()
}

REGISTER_BENCHMARK_FOR_EACH(ToString,
  ("/int/10", IntToStringConversionTest, 10),
  ("/int/1000", IntToStringConversionTest, 1000),
  ("/double/10", DoubleToStringConversionTest, 10),
  ("/double/1000", DoubleToStringConversionTest, 1000)
)