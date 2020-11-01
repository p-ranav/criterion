#pragma once
#include <criterion/details/benchmark_result.hpp>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>

namespace criterion {

class asciidoc_writer {
public:
  static bool write_results(const std::string &filename,
                            const std::unordered_map<std::string, benchmark_result> &results) {
    bool result{false};
    std::ofstream os(filename);
    if (os.is_open()) {

      os << ".Criterion Benchmark Results\n";
      os << "[cols=\"<,>,>,>,>,>,>,>\", options=\"header\"]\n";
      os << "|=====================================================================================================================================================================================\n";
      os << "| Name | Warmup Runs | Benchmark Runs | Iterations per Second | Best Estimate Mean (ns) | Best Estimate RSD (%) | Overall Best Execution Time (ns) | Overall Worst Execution Time (ns)\n";

      for (const auto &kvpair : results) {
        os << kvpair.second.to_md();
        os << "\n";
      }
      os << "|======================================================================================================================================================================================\n";
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion