#pragma once
#include <criterion/details/benchmark.hpp>
#include <criterion/details/benchmark_result.hpp>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_map>

namespace criterion {

class csv_writer {
public:
  static bool write_results(const std::string &filename,
                            const std::unordered_map<std::string, benchmark_result> &results) {
    bool result{false};
    std::ofstream os(filename);
    if (os.is_open()) {
      os << "name,warmup_runs,iterations,mean_execution_time_execution_time,best_estimate_rsd,"
            "fastest_execution_time,slowest_execution_time\n";
      for (const auto &name : benchmark::benchmark_execution_order) {
        const auto &this_result = results.at(name);
        os << this_result.to_csv() << "\n";
      }
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion