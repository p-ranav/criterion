#pragma once
#include <sstream>
#include <string>

namespace criterion {

struct benchmark_result {
  std::string name;
  std::size_t num_warmup_runs;
  std::size_t num_runs;
  std::size_t num_iterations;
  long double best_estimate_mean;
  long double best_estimate_rsd;
  long double overall_best_execution_time;
  long double overall_worst_execution_time;

  static std::string duration_to_string(const long double &ns) {
    std::stringstream os;
    if (ns < 1E3) {
      os << std::setprecision(3) << ns << "ns";
    } else if (ns < 1E6) {
      os << std::setprecision(3) << (ns / 1E3) << "us";
    } else if (ns < 1E9) {
      os << std::setprecision(3) << (ns / 1E6) << "ms";
    } else {
      os << std::setprecision(3) << (ns / 1E9) << "s";
    }
    return os.str();
  }

  static std::string csv_header() {
    return "name,warmup_runs,benchmark_runs,iterations_per_run,best_estimate_mean,best_estimate_rsd,overall_best_execution_time,overall_worst_execution_time";
  }

  std::string to_csv() const {
    std::stringstream os;

    os << '"' << name 
       << "\",";
    os << std::setprecision(3)
       << num_warmup_runs 
       << ','
       << num_runs
       << ','
       << num_iterations
       << ','
       << best_estimate_mean
       << ','
       << best_estimate_rsd
       << ','
       << overall_best_execution_time
       << ','
       << overall_worst_execution_time;

    return os.str();
  }

  std::string to_json() const {
    std::stringstream os;

    os << "    {\n";
    os << "      \"name\": \"" << name << "\",\n";
    os << "      \"warmup_runs\": " << num_warmup_runs << ",\n";
    os << "      \"benchmark_runs\": " << num_runs << ",\n";
    os << "      \"iterations_per_run\": " << num_iterations << ",\n";
    os << "      \"best_estimate_mean\": " << best_estimate_mean << ",\n";
    os << "      \"best_estimate_rsd\": " << best_estimate_rsd << ",\n";
    os << "      \"overall_best_execution_time\": " << overall_best_execution_time << ",\n";
    os << "      \"overall_worst_execution_time\": " << overall_worst_execution_time << "\n";
    os << "    }";

    return os.str();
  }

  std::string to_md() const {
    std::stringstream os;
    os << "|" << name;
    os << "|" << num_warmup_runs;
    os << "|" << num_runs;
    os << "|" << num_iterations;
    os << "|" << duration_to_string(best_estimate_mean);
    os << "|" << best_estimate_rsd << "%";
    os << "|" << duration_to_string(overall_best_execution_time);
    os << "|" << duration_to_string(overall_worst_execution_time);
    return os.str();
  }
};

} // namespace criterion