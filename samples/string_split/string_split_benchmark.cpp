#include <string>
#include <criterion/criterion.hpp>

struct StringAlgorithms {
  static std::vector<std::string> split(const std::string& string, const std::string& delimiter);
};

BENCHMARK(StringSplit_CSV)
{
  SETUP_BENCHMARK(
    static std::string greet = "Year,Make,Model,Description,Price\n1997,Ford,E350,\"ac, abs, moon\",3000.00";
  )
  auto result = StringAlgorithms::split(greet, ",");
}