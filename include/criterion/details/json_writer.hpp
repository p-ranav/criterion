#pragma once
#include <criterion/details/benchmark_result.hpp>
#include <criterion/details/benchmark.hpp>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <string>

namespace criterion {

class json_writer {
public:
  static bool write_results(const std::string &filename,
                            const std::unordered_map<std::string, benchmark_result> &results) {
    bool result{false};
    std::ofstream os(filename);
    if (os.is_open()) {
      os << "{\n";
      os << "  \"benchmarks\": [\n";

      bool first{true};
      for (const auto &name : benchmark::benchmark_execution_order) {
        const auto& this_result = results.at(name);
        if (first) {
          first = false;
        } else {
          os << ",\n";
        }
        os << this_result.to_json();
      }

      os << "\n";
      os << "  ]\n";
      os << "}";
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion