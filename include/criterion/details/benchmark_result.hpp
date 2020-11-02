#pragma once
#include <sstream>
#include <string>

namespace criterion {

struct benchmark_result {
  std::string name;
  std::size_t num_warmup_runs;
  std::size_t num_runs;
  std::size_t num_iterations;

  long double lowest_rsd;       // Lowest relative standard deviation (RSD)
  long double lowest_rsd_mean;  // mean @ lowest RSD
  std::size_t lowest_rsd_index; // which run had the lowest RSD best estimate

  long double warmup_execution_time;  // execution time during warmup
  long double mean_execution_time;    // global mean execution time
  long double fastest_execution_time; // global best execution time
  long double slowest_execution_time; // global worst execution time

  long double average_iteration_performance; // iterations per second in the average case
  long double fastest_iteration_performance; // iterations per second in the fastest case
  long double slowest_iteration_performance; // iterations per second in the slowest case

  std::string to_csv() const {
    std::stringstream os;

    os << '"' << name << "\",";
    os << std::fixed << num_warmup_runs << ',' << num_runs * num_iterations << ','
       << std::setprecision(0) << mean_execution_time << ',' << std::setprecision(2) << lowest_rsd
       << ',' << std::setprecision(0) << fastest_execution_time << ',' << std::setprecision(0)
       << slowest_execution_time;

    return os.str();
  }

  std::string to_json() const {
    std::stringstream os;
    os << std::fixed;
    os << "    {\n";
    os << "      \"name\": \"" << name << "\",\n";
    os << "      \"warmup_runs\": " << num_warmup_runs << ",\n";
    os << "      \"iterations\": " << num_runs * num_iterations << ",\n";
    os << "      \"mean_execution_time\": " << std::setprecision(0) << mean_execution_time << ",\n";
    os << "      \"best_estimate_rsd\": " << std::setprecision(2) << lowest_rsd << ",\n";
    os << "      \"fastest_execution_time\": " << std::setprecision(0) << fastest_execution_time
       << ",\n";
    os << "      \"slowest_execution_time\": " << std::setprecision(0) << slowest_execution_time
       << "\n";
    os << "    }";

    return os.str();
  }

  std::string to_md() const {
    std::stringstream os;
    os << std::fixed;
    os << "|" << name;
    os << "|" << num_warmup_runs;
    os << "|" << num_runs * num_iterations;
    os << "|" << std::setprecision(0) << mean_execution_time;
    os << "|" << std::setprecision(2) << lowest_rsd;
    os << "|" << std::setprecision(0) << fastest_execution_time;
    os << "|" << std::setprecision(0) << slowest_execution_time;
    return os.str();
  }
};

} // namespace criterion