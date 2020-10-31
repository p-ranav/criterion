#include <criterion/criterion.hpp>
#include <string>

struct StringAlgorithms {
  static std::vector<std::string> split(const std::string &string, const std::string &delimiter);
};

BENCHMARK(StringSplit, std::string) 
{
  SETUP_BENCHMARK(
    static std::string greet = GET_ARGUMENT(0);
  )
  auto result = StringAlgorithms::split(greet, ",");
}
INVOKE_BENCHMARK(StringSplit, 
  "/csv", "Year,Make,Model,Description,Price\n1997,Ford,E350,\"ac, abs, moon\",3000.00")