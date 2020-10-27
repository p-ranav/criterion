#include <string>
#include <benchmark/benchmark.hpp>

static auto split(const std::string& string, const std::string& delimiter)
    -> std::vector<std::string> {
  std::vector<std::string> result;

  if (string.empty() || delimiter.empty()) {
    return result;
  }

  auto delimiter_position = string.find(delimiter);
  if (delimiter_position != std::string::npos) {
    result.push_back(string.substr(0, delimiter_position));

    // extend result with sub-result
    auto subresult =
        split(string.substr(delimiter_position + delimiter.size()), delimiter);
    result.reserve(result.size() +
                   std::distance(subresult.begin(), subresult.end()));
    result.insert(result.end(), subresult.begin(), subresult.end());
  } else {
    result.push_back(string);
  }
  return result;
}

BENCHMARK("string split")
{
  std::string greet = "Year,Make,Model,Description,Price\n1997,Ford,E350,\"ac, abs, moon\",3000.00";
  auto result = split(greet, ",");
}

// Example output on Macbook Pro 2019
// ✔ string_split.cpp:28 [string split] [01m:07s<00m:00s] μ = 3.64us ± 0.294%