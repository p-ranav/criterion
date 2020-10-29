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

BENCHMARK_TEMPLATE(to_string, /* parameters -> */ std::function<std::vector<std::string>(int)>, int)
{
  SETUP_BENCHMARK(
    const auto args = BENCHMARK_ARGUMENTS;
    const auto fn = std::get<0>(args);
    const auto size = std::get<1>(args);
  )
  fn(size);
  TEARDOWN_BENCHMARK()
}

RUN_BENCHMARK_TEMPLATE(to_string, "/int/10", IntToStringConversionTest, 10)
RUN_BENCHMARK_TEMPLATE(to_string, "/int/1000", IntToStringConversionTest, 1000)
RUN_BENCHMARK_TEMPLATE(to_string, "/double/10", DoubleToStringConversionTest, 10)
RUN_BENCHMARK_TEMPLATE(to_string, "/double/1000", DoubleToStringConversionTest, 1000)