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

  std::string to_csv() const {
    std::stringstream os;

    os << '"' << name 
       << "\",";
    os << std::fixed
       << num_warmup_runs 
       << ','
       << num_runs
       << ','
       << num_iterations
       << ','
       << std::setprecision(0) << best_estimate_mean
       << ','
       << std::setprecision(2) << best_estimate_rsd
       << ','
       << std::setprecision(0) << overall_best_execution_time
       << ','
       << std::setprecision(0) << overall_worst_execution_time;

    return os.str();
  }

  std::string to_json() const {
    std::stringstream os;
    os << std::fixed;
    os << "    {\n";
    os << "      \"name\": \"" << name << "\",\n";
    os << "      \"warmup_runs\": " << num_warmup_runs << ",\n";
    os << "      \"benchmark_runs\": " << num_runs << ",\n";
    os << "      \"iterations_per_run\": " << num_iterations << ",\n";
    os << "      \"best_estimate_mean_execution_time\": " << std::setprecision(0) << best_estimate_mean << ",\n";
    os << "      \"best_estimate_rsd\": " << std::setprecision(2) << best_estimate_rsd << ",\n";
    os << "      \"overall_best_execution_time\": " << std::setprecision(0) << overall_best_execution_time << ",\n";
    os << "      \"overall_worst_execution_time\": " << std::setprecision(0) << overall_worst_execution_time << "\n";
    os << "    }";

    return os.str();
  }

  std::string to_md() const {
    std::stringstream os;
    os << std::fixed;
    os << "|" << name;
    os << "|" << num_warmup_runs;
    os << "|" << num_runs;
    os << "|" << num_iterations;
    os << "|" << std::setprecision(0) << best_estimate_mean;
    os << "|" << std::setprecision(2) << best_estimate_rsd;
    os << "|" << std::setprecision(0) << overall_best_execution_time;
    os << "|" << std::setprecision(0) << overall_worst_execution_time;
    return os.str();
  }
};

} // namespace criterion