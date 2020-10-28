#include <vector>
#include <benchmark/registration.hpp>

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

BENCHMARK("int_to_string/10")
{
  IntToStringConversionTest(10);
}

BENCHMARK("int_to_string/1000")
{
  IntToStringConversionTest(1000);
}

BENCHMARK("double_to_string/10")
{
  DoubleToStringConversionTest(10);
}

BENCHMARK("double_to_string/1000")
{
  DoubleToStringConversionTest(1000);
}