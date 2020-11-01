#pragma once
#include <criterion/details/benchmark_result.hpp>
#include <criterion/details/benchmark.hpp>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>

namespace criterion {

class md_writer {
public:
  static bool write_results(const std::string &filename,
                            const std::unordered_map<std::string, benchmark_result> &results) {
    bool result{false};
    std::ofstream os(filename);
    if (os.is_open()) {

      os << "| Name | Warmup Runs | Benchmark Runs | Iterations per Second | Best Estimate Mean (ns) | Best Estimate RSD (%) | Overall Best Execution Time (ns) | Overall Worst Execution Time (ns) |\n";
      os << "|------|------------:|---------------:|----------------------:|------------------------:|----------------------:|---------------------------------:|----------------------------------:|\n";

      for (const auto &name : benchmark::benchmark_execution_order) {
        const auto& this_result = results.at(name);
        os << this_result.to_md();
        os << "\n";
      }
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion