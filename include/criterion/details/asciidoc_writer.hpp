#pragma once
#include <criterion/details/benchmark_result.hpp>
#include <criterion/details/benchmark.hpp>
#include <ctime>
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

      std::time_t t = std::time(nullptr);
      os << ".Criterion Benchmark Results (" << std::put_time(std::gmtime(&t), "%c %Z") << ")\n";
      os << "[cols=\"<,>,>,>,>,>,>,>\", options=\"header\"]\n";
      os << "|=====================================================================================================================================================================================\n";
      os << "| Name | Warmup Runs | Benchmark Runs | Iterations per Second | Best Estimate Mean (ns) | Best Estimate RSD (%) | Overall Best Execution Time (ns) | Overall Worst Execution Time (ns)\n";

      for (const auto &name : benchmark::benchmark_execution_order) {
        const auto& this_result = results.at(name);
        os << this_result.to_md();
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