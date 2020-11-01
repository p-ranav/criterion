#pragma once
#include <criterion/details/benchmark_result.hpp>
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
      os << benchmark_result::csv_header() << "\n";
      for (const auto &kvpair : results) {
        os << kvpair.second.to_csv() << "\n";
      }
      result = true;
    }
    os.close();
    return result;
  }
};

} // namespace criterion