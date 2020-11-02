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
    os << std::fixed << std::setprecision(2)
       << "    {\n"
       << "      \"name\": \"" << name << "\",\n"
       << "      \"warmup_runs\": " << num_warmup_runs << ",\n"
       << "      \"iterations\": " << num_runs * num_iterations << ",\n"
       << "      \"mean_execution_time\": " << mean_execution_time << ",\n"
       << "      \"fastest_execution_time\": " << fastest_execution_time << ",\n"
       << "      \"slowest_execution_time\": " << slowest_execution_time << ",\n"
       << "      \"lowest_rsd_execution_time\": " << lowest_rsd_mean << ",\n"
       << "      \"lowest_rsd_percentage\": " << lowest_rsd << ",\n"
       << "      \"lowest_rsd_index\": " << lowest_rsd_index << ",\n"
       << "      \"average_iteration_performance\": " << average_iteration_performance << ",\n"
       << "      \"fastest_iteration_performance\": " << fastest_iteration_performance << ",\n"
       << "      \"slowest_iteration_performance\": " << slowest_iteration_performance << "\n"
       << "    }";

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