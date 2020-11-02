#pragma once
#include <array>
#include <criterion/details/benchmark_result.hpp>
#include <criterion/details/indicators.hpp>
#include <iomanip>
#include <sstream>
#include <string>

namespace criterion {

class console_writer {
  static std::string duration_to_string(const long double &ns) {
    const auto duration = std::abs(ns);
    std::stringstream os;
    if (duration < 1E3) {
      os << std::fixed << std::setprecision(2) << duration << " ns";
    } else if (duration < 1E6) {
      os << std::fixed << std::setprecision(2) << (duration / 1E3) << " us";
    } else if (duration < 1E9) {
      os << std::fixed << std::setprecision(2) << (duration / 1E6) << " ms";
    } else {
      os << std::fixed << std::setprecision(2) << (duration / 1E9) << " s";
    }

    std::string result{""};
    if (ns < 0) {
      result += "-";
    }
    result += os.str();
    return result;
  }

  static std::string ordinal(std::size_t n) {
    static const std::array<std::string, 10> ends{"th", "st", "nd", "rd", "th",
                                                  "th", "th", "th", "th", "th"};
    if (((n % 100) >= 11) && ((n % 100) <= 13)) {
      return std::to_string(n) + "th";
    } else {
      return std::to_string(n) + ends[n % 10];
    }
  }

public:
  static void write_result(const benchmark_result &result) {
    std::cout << termcolor::bold << termcolor::green << " ✓ " << result.name << termcolor::reset
              << "\n";

    std::cout << "    " << termcolor::bold << termcolor::underline << "Configuration"
              << termcolor::reset << "\n";

    std::cout << "      " << result.num_warmup_runs << " warmup runs, " << result.num_runs
              << (result.num_runs > 1 ? " benchmark runs, " : " benchmark run, ") << result.num_iterations
              << " iterations per run\n";

    std::cout << "    " << termcolor::bold << termcolor::underline << "Execution Time"
              << termcolor::reset << "\n";

    std::cout << termcolor::yellow << termcolor::bold << termcolor::italic << "      Average      "
              << std::right << std::setw(10) << duration_to_string(result.mean_execution_time)
              << termcolor::reset << "\n";

    const auto best_mean_difference = result.fastest_execution_time - result.mean_execution_time;
    const auto best_mean_percentage_difference =
        (best_mean_difference / static_cast<long double>(result.mean_execution_time) * 100.0);

    std::cout << "      Fastest      " << std::right << std::setw(10)
              << duration_to_string(result.fastest_execution_time);
    std::cout << " (" << termcolor::green << duration_to_string(best_mean_difference) << ", "
              << std::setprecision(2) << std::fixed << best_mean_percentage_difference << " %"
              << termcolor::reset << ")"
              << "\n";

    const auto worst_mean_difference = result.slowest_execution_time - result.mean_execution_time;
    const auto worst_mean_percentage_difference =
        (worst_mean_difference / static_cast<long double>(result.mean_execution_time) * 100.0);

    std::cout << "      Slowest      " << std::right << std::setw(10)
              << duration_to_string(result.slowest_execution_time);
    std::cout << " (" << termcolor::red << duration_to_string(worst_mean_difference) << ", "
              << std::setprecision(2) << std::fixed << worst_mean_percentage_difference << " %"
              << termcolor::reset << ")"
              << "\n";

    std::cout << termcolor::bold << termcolor::white << "      Lowest RSD   " << std::right
              << std::setw(10) << duration_to_string(result.lowest_rsd_mean) << " ± "
              << std::setprecision(2) << result.lowest_rsd << "%"
              << " (" << ordinal(result.lowest_rsd_index) << " run)" << termcolor::reset << "\n";

    std::cout << "    " << termcolor::bold << termcolor::underline << "Performance"
              << termcolor::reset << "\n";

    std::cout << "      Average    " << std::setprecision(2) << std::fixed << std::right
              << std::setw(10) << result.average_iteration_performance << " iterations/s"
              << termcolor::reset << "\n";

    const auto best_mean_iterations_difference =
        result.average_iteration_performance - result.fastest_iteration_performance;
    const auto best_mean_iterations_percentage_difference =
        (best_mean_iterations_difference /
         static_cast<long double>(result.average_iteration_performance) * 100.0);

    std::cout << "      Fastest    " << std::setprecision(2) << std::fixed << std::right
              << std::setw(10) << result.fastest_iteration_performance << " iterations/s"
              << termcolor::reset;
    std::cout << " (" << termcolor::green << std::setprecision(2) << std::fixed
              << best_mean_iterations_difference << " iterations/s, "
              << best_mean_iterations_percentage_difference << " %" << termcolor::reset << ")"
              << "\n";

    const auto worst_mean_iterations_difference =
        result.average_iteration_performance - result.slowest_iteration_performance;
    const auto worst_mean_iterations_percentage_difference =
        (worst_mean_iterations_difference /
         static_cast<long double>(result.average_iteration_performance) * 100.0);

    std::cout << "      Slowest    " << std::setprecision(2) << std::fixed << std::right
              << std::setw(10) << result.slowest_iteration_performance << " iterations/s"
              << termcolor::reset;
    std::cout << " (" << termcolor::red << std::setprecision(2) << std::fixed
              << worst_mean_iterations_difference << " iterations/s, "
              << worst_mean_iterations_percentage_difference << " %" << termcolor::reset << ")"
              << "\n";

    std::cout << "\n";
  }
};

} // namespace criterion