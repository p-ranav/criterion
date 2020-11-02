#pragma once
#include <criterion/details/benchmark.hpp>
#include <criterion/details/benchmark_result.hpp>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>

namespace criterion {

class md_writer {
public:
  static bool write_results(const std::string &filename,
                            const std::unordered_map<std::string, benchmark_result> &results) {
    bool result{false};
    std::ofstream os(filename);
    if (os.is_open()) {

      os << "|Name            |Warmup Runs|Iterations|Mean Execution Time (ns)|Fastest Execution "
            "Time (ns)|Slowest Execution Time (ns)|Lowest RSD Execution Time (ns)|Lowest RSD "
            "(%)|Lowest RSD Index|Average Iteration Performance (iterations/s)|Fastest Iteration "
            "Performance (iterations/s)|Slowest Iteration Performance (iterations/s)|\n";
      os << "|:---------------|----------:|---------:|-----------------------:|--------------------"
            "------:|--------------------------:|-----------------------------:|-------------:|----"
            "-----------:|-------------------------------------------:|----------------------------"
            "---------------:|-------------------------------------------:|\n";

      for (const auto &name : benchmark::benchmark_execution_order) {
        const auto &this_result = results.at(name);
        os << this_result.to_md();
      }
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion