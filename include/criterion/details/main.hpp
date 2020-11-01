#pragma once
#include <criterion/details/indicators.hpp>
#include <criterion/details/macros.hpp>
#include <criterion/details/structopt.hpp>
#include <criterion/details/help.hpp>
#include <criterion/details/csv_writer.hpp>
#include <criterion/details/json_writer.hpp>

static inline void signal_handler(int signal) {
  indicators::show_console_cursor(true);
  std::cout << termcolor::reset;
  exit(signal);
}

namespace criterion {

struct options {

  struct export_options : structopt::sub_command {
    enum class format_type { csv, json };

    // Export format
    format_type format;

    // Export filename
    std::string filename;

    // Prints help
    std::optional<bool> help = false;
  };

  // --export_results csv result.csv
  // --export_results json foo.json
  std::optional<export_options> export_results;

  // Prints help
  std::optional<bool> help = false;

  // Remaining arguments
  std::vector<std::string> remaining;
};

}

STRUCTOPT(criterion::options::export_options, format, filename);
STRUCTOPT(criterion::options, export_results, help, remaining);

static inline int criterion_main(int argc, char *argv[]) { 

  std::signal(SIGTERM, signal_handler);                                                          
  std::signal(SIGSEGV, signal_handler);                                                          
  std::signal(SIGINT, signal_handler);                                                           
  std::signal(SIGILL, signal_handler);                                                           
  std::signal(SIGABRT, signal_handler);                                                          
  std::signal(SIGFPE, signal_handler);

  try {
    auto options = structopt::app("criterion").parse<criterion::options>(argc, argv);

    if (options.help.value() == true || (options.export_results.has_value() && options.export_results.value().help.value() == true)) {
      print_criterion_help();
      exit(0);
    }
    else if (!options.remaining.empty()) {
      std::cout << termcolor::bold << termcolor::red;
      std::cout << "Error: Unrecognized argument \"";
      std::cout << options.remaining[0];
      std::cout << "\"" << termcolor::reset << "\n";
      print_criterion_help();
      exit(1);
    }

    // Run benchmarks
    criterion::benchmark_registration_helper_struct::execute_registered_benchmarks();   

    if (options.export_results.has_value()) {
      auto export_options = options.export_results.value();
      if (export_options.format == criterion::options::export_options::format_type::csv) {
        // CSV export
        criterion::csv_writer::write_results(export_options.filename, criterion::benchmark::results);
      }
      else if (export_options.format == criterion::options::export_options::format_type::json) {
        // JSON export
        criterion::json_writer::write_results(export_options.filename, criterion::benchmark::results);
      }
    }


  } catch (structopt::exception& e) {
    std::cout << termcolor::bold << termcolor::red << e.what() << termcolor::reset << "\n";
    print_criterion_help();
    exit(1);
  }                                                      
  return 0;        
}

#define CRITERION_BENCHMARK_MAIN(...)\
  int main(int argc, char *argv[]) {\
    criterion_main(argc, argv);\
  }