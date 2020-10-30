#pragma once
#include <criterion/details/csv2.hpp>
#include <sstream>
#include <string>
#include <map>
#include <criterion/details/benchmark_result.hpp>

namespace criterion {

class csv_writer {

  static std::vector<std::string> to_csv_row(const benchmark_result& result) {
    return std::vector<std::string> {
      "\"" + result.name + "\"",
      std::to_string(result.num_warmup_runs),
      std::to_string(result.num_runs),
      std::to_string(result.num_iterations),
      // nanoseconds
      std::to_string(result.best_estimate_mean),
      std::to_string(result.best_estimate_rsd),
      std::to_string(result.overall_best_execution_time),
      std::to_string(result.overall_worst_execution_time),
    };
  }

public:
  static bool write_results(const std::string& filename, const std::map<std::string, benchmark_result>& results) {
    bool result{false};

    std::ofstream stream(filename);

    using namespace csv2;
    Writer<delimiter<','>> writer(stream);
    if (stream.is_open()) {
      result = true;

      writer.write_row(std::vector<std::string>{
        "\"Name\"",
        "\"Warmup Runs\"",
        "\"Benchmark Runs\"",
        "\"Iterations per Run\"",
        "\"Best Estimate Mean (ns)\"",
        "\"Best Estimate RSD (%)\"",
        "\"Overall Best Execution Time (ns)\"",
        "\"Overall Worst Execution Time (ns)\""
      });

      for (const auto& kvpair: results) {
        writer.write_row(to_csv_row(kvpair.second));
      }
    }

    stream.close();
    return result;
  }
};

}