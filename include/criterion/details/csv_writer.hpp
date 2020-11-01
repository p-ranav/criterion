#pragma once
#include <criterion/details/benchmark_result.hpp>
#include <criterion/details/benchmark.hpp>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>

namespace criterion {

class csv_writer {
public:
  static bool write_results(const std::string &filename,
                            const std::unordered_map<std::string, benchmark_result> &results) {
    bool result{false};
    std::ofstream os(filename);
    if (os.is_open()) {
      os << "name,warmup_runs,benchmark_runs,iterations_per_run,best_estimate_mean_execution_time,best_estimate_rsd,overall_best_execution_time,overall_worst_execution_time\n";
      for (const auto &name : benchmark::benchmark_execution_order) {
        const auto& this_result = results.at(name);
        os << this_result.to_csv() << "\n";
      }
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion