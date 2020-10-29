#pragma once
#include <benchmark/for_each_macro.hpp>
#include <benchmark/registration.hpp>
#include <benchmark/benchmark_template.hpp>
#include <benchmark/benchmark_config.hpp>
#include <unordered_map>

void register_benchmark_template(const benchmark_config &config);

struct register_benchmark_template_data {
  static std::unordered_multimap<std::string, benchmark_config> & registered_benchmark_templates();
};

template <class ArgTuple>
void execute_registered_benchmark_template(const std::string& template_name, const std::string& instance_name, ArgTuple& arg_tuple) {
  for (auto& [k, v] : register_benchmark_template_data::registered_benchmark_templates()) {
    if (k == template_name) {
      register_benchmark(
        benchmark_config{ 
          .name = template_name, 
          .fn = v.fn,
          .parameterized_instance_name = instance_name,
          .parameters = (void *)(&arg_tuple)
        }
      );
    }
  }
}

#define BENCHMARK_TEMPLATE(Name, ...) \
  typedef std::tuple<__VA_ARGS__> CONCAT(Name, BenchmarkParameters); \
  namespace detail {                                                           \
  /* forward declare the benchmark function that we define later */            \
  template <class T = CONCAT(Name, BenchmarkParameters)> \
  struct CONCAT(__benchmark_function_wrapper__, __LINE__) {                    \
    static void CONCAT(_registered_fun_, __LINE__)(                            \
        std::chrono::steady_clock::time_point &,                               \
        std::optional<std::chrono::steady_clock::time_point> &, \
        void *);       \
  };                                                                           \
                                                                               \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(_register_struct_, __LINE__) {                                 \
    CONCAT(_register_struct_, __LINE__)() { /* called once before main */      \
      register_benchmark_template(benchmark_config{                            \
          .name = #Name,                                                        \
          .fn = CONCAT(__benchmark_function_wrapper__,                         \
                       __LINE__)<CONCAT(Name, BenchmarkParameters)>::CONCAT(_registered_fun_, __LINE__)});        \
    }                                                                          \
  } CONCAT(_register_struct_instance_, __LINE__);                              \
  }                                                                            \
  }                                                                            \
                                                                               \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */         \
  template <class T> \
  void detail::CONCAT(__benchmark_function_wrapper__, __LINE__)<T>::CONCAT(       \
      _registered_fun_, __LINE__)(                                             \
      [[maybe_unused]] std::chrono::steady_clock::time_point &                 \
          __benchmark_start_timestamp,                                         \
      [[maybe_unused]] std::optional<std::chrono::steady_clock::time_point> &  \
          __benchmark_teardown_timestamp, \
      [[maybe_unused]] void * __benchmark_parameters)

#define BENCHMARK_ARGUMENTS \
  *((T *)__benchmark_parameters);

#define RUN_BENCHMARK_TEMPLATE(TemplateName, InstanceName, ...) \
  \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  static CONCAT(TemplateName, BenchmarkParameters) CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__) = {__VA_ARGS__}; \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(_instantiation_struct_, __LINE__) {                                 \
    CONCAT(_instantiation_struct_, __LINE__)() { /* called once before main */      \
      execute_registered_benchmark_template<CONCAT(TemplateName, BenchmarkParameters)>(#TemplateName, InstanceName, CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__)); \
    }                                                                          \
  } CONCAT(_instantiation_struct_instance_, __LINE__);                              \
  }