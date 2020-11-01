#pragma once
#include <criterion/details/benchmark_result.hpp>
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

      os << "| Name | Warmup Runs | Benchmark Runs | Iterations per Second | Best Estimate Mean | Best Estimate RSD (%) | Overall Best Execution Time | Overall Worst Execution Time |\n";
      os << "|------|------------:|---------------:|----------------------:|-------------------:|----------------------:|----------------------------:|-----------------------------:|\n";

      for (const auto &kvpair : results) {
        os << kvpair.second.to_md();
        os << "\n";
      }
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion