#pragma once
#include <criterion/details/asciidoc_writer.hpp>
#include <criterion/details/csv_writer.hpp>
#include <criterion/details/help.hpp>
#include <criterion/details/indicators.hpp>
#include <criterion/details/json_writer.hpp>
#include <criterion/details/macros.hpp>
#include <criterion/details/md_writer.hpp>
#include <criterion/details/structopt.hpp>
#include <cstring>

static inline void signal_handler(int signal) {
  std::cout << termcolor::reset;
  exit(signal);
}

namespace criterion {

struct options {

  struct export_options : structopt::sub_command {
    enum class format_type { csv, json, md, asciidoc };

    // Export format
    format_type format;

    // Export filename
    std::string filename;
  };

  // List available benchmarks
  std::optional<bool> list = false;

  // List available benchmarks, filtered by user-provided regex string
  std::optional<std::string> list_filtered;

  // Run filtered benchmarks, filtered by user-provided regex string
  std::optional<std::string> run_filtered;

  // --export_results csv result.csv
  // --export_results json foo.json
  std::optional<export_options> export_results;

  // Prints help
  std::optional<bool> help = false;

  // Remaining arguments
  std::vector<std::string> remaining;
};

} // namespace criterion

STRUCTOPT(criterion::options::export_options, format, filename);
STRUCTOPT(criterion::options, list, list_filtered, run_filtered, export_results, help, remaining);

static inline int criterion_main(int argc, char *argv[]) {
  const auto program_name = argv[0];

  std::signal(SIGTERM, signal_handler);
  std::signal(SIGSEGV, signal_handler);
  std::signal(SIGINT, signal_handler);
  std::signal(SIGILL, signal_handler);
  std::signal(SIGABRT, signal_handler);
  std::signal(SIGFPE, signal_handler);

  try {
    auto options = structopt::app(program_name).parse<criterion::options>(argc, argv);

    if (options.help.value() == true) {
      print_criterion_help(program_name);
      exit(0);
    } else if (options.list.value() == true) {
      criterion::benchmark_registration_helper_struct::list_registered_benchmarks();
      exit(0);
    } else if (options.list_filtered.has_value()) {
      criterion::benchmark_registration_helper_struct::list_filtered_registered_benchmarks(
          options.list_filtered.value());
      exit(0);
    } else if (!options.remaining.empty()) {
      std::cout << termcolor::bold << termcolor::red;
      std::cout << "Error: Unrecognized argument \"";
      std::cout << options.remaining[0];
      std::cout << "\"" << termcolor::reset << "\n";
      print_criterion_help(program_name);
      exit(1);
    }

    // Run benchmarks
    if (options.run_filtered.has_value()) { // Run filtered
      criterion::benchmark_registration_helper_struct::execute_filtered_registered_benchmarks(
          options.run_filtered.value());
    } else {
      criterion::benchmark_registration_helper_struct::execute_registered_benchmarks();
    }

    if (options.export_results.has_value()) {
      auto export_options = options.export_results.value();
      if (export_options.format == criterion::options::export_options::format_type::csv) {
        // CSV export
        criterion::csv_writer::write_results(export_options.filename,
                                             criterion::benchmark::results);
      } else if (export_options.format == criterion::options::export_options::format_type::json) {
        // JSON export
        criterion::json_writer::write_results(export_options.filename,
                                              criterion::benchmark::results);
      } else if (export_options.format == criterion::options::export_options::format_type::md) {
        // Markdown export
        criterion::md_writer::write_results(export_options.filename, criterion::benchmark::results);
      } else if (export_options.format ==
                 criterion::options::export_options::format_type::asciidoc) {
        // Markdown export
        criterion::asciidoc_writer::write_results(export_options.filename,
                                                  criterion::benchmark::results);
      }
    }

  } catch (structopt::exception &e) {
    const auto message = e.what();
    if (message && std::strlen(message) > 0)
      std::cout << termcolor::bold << termcolor::red << message << termcolor::reset << "\n";
    print_criterion_help(program_name);
    exit(1);
  }
  return 0;
}

#define CRITERION_BENCHMARK_MAIN(...)                                                              \
  int main(int argc, char *argv[]) { criterion_main(argc, argv); }