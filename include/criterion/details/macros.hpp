#pragma once
#include <chrono>
#include <criterion/details/benchmark.hpp>
#include <criterion/details/benchmark_config.hpp>
#include <criterion/details/csv_writer.hpp>
#include <functional>
#include <string>
#include <unordered_map>

namespace criterion {

struct benchmark_registration_helper_struct {
  static std::vector<benchmark_config> &registered_benchmarks() {
    static std::vector<benchmark_config> v;
    return v;
  }

  static void register_benchmark(const benchmark_config &config) {
    registered_benchmarks().push_back(config);
  }

  static void execute_registered_benchmarks() {
    for (const auto &config : registered_benchmarks()) {
      benchmark{config}.run();
    }
  }
};

} // namespace criterion

#define SETUP_BENCHMARK(...)                                                                       \
  __VA_ARGS__                                                                                      \
  __benchmark_start_timestamp =                                                                    \
      std::chrono::steady_clock::now(); // updated benchmark start timestamp

#define TEARDOWN_BENCHMARK(...)                                                                    \
  __benchmark_teardown_timestamp = std::chrono::steady_clock::now();                               \
  __VA_ARGS__

namespace criterion {

struct benchmark_template_registration_helper_struct {
  static std::unordered_multimap<std::string, benchmark_config> &registered_benchmark_templates() {
    static std::unordered_multimap<std::string, benchmark_config> m;
    return m;
  }

  static void register_benchmark_template(const benchmark_config &config) {
    registered_benchmark_templates().insert({config.name, config});
  }

  template <class ArgTuple>
  static void execute_registered_benchmark_template(const std::string &template_name,
                                                    const std::string &instance_name,
                                                    ArgTuple &arg_tuple) {
    for (auto &[k, v] : registered_benchmark_templates()) {
      if (k == template_name) {
        benchmark_registration_helper_struct::register_benchmark(
            benchmark_config{.name = template_name,
                             .fn = v.fn,
                             .parameterized_instance_name = instance_name,
                             .parameters = (void *)(&arg_tuple)});
      }
    }
  }
};

} // namespace criterion

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define BENCHMARK_WITHOUT_PARAMETERS(Name)                                                         \
  typedef std::tuple<> CONCAT(Name, BenchmarkParameters);                                          \
  namespace detail {                                                                               \
  /* forward declare the benchmark function that we define later */                                \
  template <class T = CONCAT(Name, BenchmarkParameters)>                                           \
  struct CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__)) {                          \
    static inline void CONCAT(Name, CONCAT(_registered_fun_, __LINE__))(                                  \
        std::chrono::steady_clock::time_point &,                                                   \
        std::optional<std::chrono::steady_clock::time_point> &, void *);                           \
  };                                                                                               \
                                                                                                   \
  namespace /* ensure internal linkage for struct */                                               \
  {                                                                                                \
  /* helper struct for static registration in ctor */                                              \
  struct CONCAT(Name, CONCAT(_register_struct_, __LINE__)) {                                       \
    CONCAT(Name, CONCAT(_register_struct_, __LINE__))() { /* called once before main */            \
      criterion::benchmark_template_registration_helper_struct::register_benchmark_template(       \
          criterion::benchmark_config{                                                             \
              .name = #Name,                                                                       \
              .fn = CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__)) <               \
                    CONCAT(Name, BenchmarkParameters) >                                            \
                    ::CONCAT(Name, CONCAT(_registered_fun_, __LINE__))});                          \
    }                                                                                              \
  } CONCAT(Name, CONCAT(_register_struct_instance_, __LINE__));                                    \
  }                                                                                                \
                                                                                                   \
  namespace /* ensure internal linkage for struct */                                               \
  {                                                                                                \
  static CONCAT(Name, BenchmarkParameters)                                                         \
      CONCAT(CONCAT(Name, _benchmark_template_parameters), __LINE__) = {};                         \
  /* helper struct for static registration in ctor */                                              \
  struct CONCAT(Name, CONCAT(_instantiation_struct_, __LINE__)) {                                  \
    CONCAT(Name, CONCAT(_instantiation_struct_, __LINE__))() { /* called once before main */       \
      criterion::benchmark_template_registration_helper_struct::                                   \
          execute_registered_benchmark_template<CONCAT(Name, BenchmarkParameters)>(                \
              #Name, "", CONCAT(CONCAT(Name, _benchmark_template_parameters), __LINE__));          \
    }                                                                                              \
  } CONCAT(Name, CONCAT(_instantiation_struct_instance_, __LINE__));                               \
  }                                                                                                \
  }                                                                                                \
                                                                                                   \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */                             \
  template <class T>                                                                               \
  void detail::CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__))<T>::CONCAT(          \
      Name, CONCAT(_registered_fun_, __LINE__))(                                                   \
      [[maybe_unused]] std::chrono::steady_clock::time_point & __benchmark_start_timestamp,        \
      [[maybe_unused]] std::optional<std::chrono::steady_clock::time_point> &                      \
          __benchmark_teardown_timestamp,                                                          \
      [[maybe_unused]] void *__benchmark_parameters)

#define BENCHMARK_WITH_PARAMETERS(Name, ...)                                                       \
  typedef std::tuple<__VA_ARGS__> CONCAT(Name, BenchmarkParameters);                               \
  namespace detail {                                                                               \
  /* forward declare the benchmark function that we define later */                                \
  template <class T = CONCAT(Name, BenchmarkParameters)>                                           \
  struct CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__)) {                          \
    static inline void CONCAT(Name, CONCAT(_registered_fun_, __LINE__))(                                  \
        std::chrono::steady_clock::time_point &,                                                   \
        std::optional<std::chrono::steady_clock::time_point> &, void *);                           \
  };                                                                                               \
                                                                                                   \
  namespace /* ensure internal linkage for struct */                                               \
  {                                                                                                \
  /* helper struct for static registration in ctor */                                              \
  struct CONCAT(Name, CONCAT(_register_struct_, __LINE__)) {                                       \
    CONCAT(Name, CONCAT(_register_struct_, __LINE__))() { /* called once before main */            \
      criterion::benchmark_template_registration_helper_struct::register_benchmark_template(       \
          criterion::benchmark_config{                                                             \
              .name = #Name,                                                                       \
              .fn = CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__)) <               \
                    CONCAT(Name, BenchmarkParameters) >                                            \
                    ::CONCAT(Name, CONCAT(_registered_fun_, __LINE__))});                          \
    }                                                                                              \
  } CONCAT(Name, CONCAT(_register_struct_instance_, __LINE__));                                    \
  }                                                                                                \
  }                                                                                                \
                                                                                                   \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */                             \
  template <class T>                                                                               \
  void detail::CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__))<T>::CONCAT(          \
      Name, CONCAT(_registered_fun_, __LINE__))(                                                   \
      [[maybe_unused]] std::chrono::steady_clock::time_point & __benchmark_start_timestamp,        \
      [[maybe_unused]] std::optional<std::chrono::steady_clock::time_point> &                      \
          __benchmark_teardown_timestamp,                                                          \
      [[maybe_unused]] void *__benchmark_parameters)

#define GET_ARGUMENT_TUPLE *((T *)__benchmark_parameters)

#define GET_ARGUMENT(index) std::get<index>(*((T *)__benchmark_parameters));

#define BENCHMARK_1(Name) BENCHMARK_WITHOUT_PARAMETERS(Name)

#define BENCHMARK_2(Name, A) BENCHMARK_WITH_PARAMETERS(Name, A)
#define BENCHMARK_3(Name, A, B) BENCHMARK_WITH_PARAMETERS(Name, A, B)
#define BENCHMARK_4(Name, A, B, C) BENCHMARK_WITH_PARAMETERS(Name, A, B, C)
#define BENCHMARK_5(Name, A, B, C, D) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D)
#define BENCHMARK_6(Name, A, B, C, D, E) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F)
#define BENCHMARK_7(Name, A, B, C, D, E, F) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F)
#define BENCHMARK_8(Name, A, B, C, D, E, F, G) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G)
#define BENCHMARK_9(Name, A, B, C, D, E, F, G, H)                                                  \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H)
#define BENCHMARK_10(Name, A, B, C, D, E, F, G, H, I)                                              \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I)
#define BENCHMARK_11(Name, A, B, C, D, E, F, G, H, I, J)                                           \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J)
#define BENCHMARK_12(Name, A, B, C, D, E, F, G, H, I, J, K)                                        \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K)
#define BENCHMARK_13(Name, A, B, C, D, E, F, G, H, I, J, K, L)                                     \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L)
#define BENCHMARK_14(Name, A, B, C, D, E, F, G, H, I, J, K, L, M)                                  \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M)
#define BENCHMARK_15(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N)                               \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N)
#define BENCHMARK_16(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O)                            \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O)
#define BENCHMARK_17(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P)                         \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P)
#define BENCHMARK_18(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q)                      \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q)
#define BENCHMARK_19(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R)                   \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R)
#define BENCHMARK_20(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S)                \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S)
#define BENCHMARK_21(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T)             \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T)
#define BENCHMARK_22(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U)          \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U)
#define BENCHMARK_23(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V)       \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V)
#define BENCHMARK_24(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W)    \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U,   \
                            V, W)
#define BENCHMARK_25(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X) \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U,   \
                            V, W, X)
#define BENCHMARK_26(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, \
                     Y)                                                                            \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U,   \
                            V, W, X, Y)
#define BENCHMARK_27(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, \
                     Y, Z)                                                                         \
  BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U,   \
                            V, W, X, Y, Z)

// The interim macro that simply strips the excess and ends up with the required macro
#define BENCHMARK_X(Name, x, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W,  \
                    X, Y, Z, FUNC, ...)                                                            \
  FUNC

// BENCHMARK macro supports no more than 26 parameters
// The macro that the programmer uses
#define BENCHMARK(...)                                                                             \
  BENCHMARK_X(, ##__VA_ARGS__, BENCHMARK_27(__VA_ARGS__), BENCHMARK_26(__VA_ARGS__),               \
              BENCHMARK_25(__VA_ARGS__), BENCHMARK_24(__VA_ARGS__), BENCHMARK_23(__VA_ARGS__),     \
              BENCHMARK_22(__VA_ARGS__), BENCHMARK_21(__VA_ARGS__), BENCHMARK_20(__VA_ARGS__),     \
              BENCHMARK_19(__VA_ARGS__), BENCHMARK_18(__VA_ARGS__), BENCHMARK_17(__VA_ARGS__),     \
              BENCHMARK_16(__VA_ARGS__), BENCHMARK_15(__VA_ARGS__), BENCHMARK_14(__VA_ARGS__),     \
              BENCHMARK_13(__VA_ARGS__), BENCHMARK_12(__VA_ARGS__), BENCHMARK_11(__VA_ARGS__),     \
              BENCHMARK_10(__VA_ARGS__), BENCHMARK_9(__VA_ARGS__), BENCHMARK_8(__VA_ARGS__),       \
              BENCHMARK_7(__VA_ARGS__), BENCHMARK_6(__VA_ARGS__), BENCHMARK_5(__VA_ARGS__),        \
              BENCHMARK_4(__VA_ARGS__), BENCHMARK_3(__VA_ARGS__), BENCHMARK_2(__VA_ARGS__),        \
              BENCHMARK_1(__VA_ARGS__))

#define REGISTER_BENCHMARK(TemplateName, InstanceName, ...)                                        \
                                                                                                   \
  namespace /* ensure internal linkage for struct */                                               \
  {                                                                                                \
  static CONCAT(TemplateName, BenchmarkParameters)                                                 \
      CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__) = {__VA_ARGS__};      \
  /* helper struct for static registration in ctor */                                              \
  struct CONCAT(TemplateName, CONCAT(_instantiation_struct_, __LINE__)) {                          \
    CONCAT(TemplateName, CONCAT(_instantiation_struct_, __LINE__))                                 \
    () { /* called once before main */                                                             \
      criterion::benchmark_template_registration_helper_struct::                                   \
          execute_registered_benchmark_template<CONCAT(TemplateName, BenchmarkParameters)>(        \
              #TemplateName, InstanceName,                                                         \
              CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__));             \
    }                                                                                              \
  } CONCAT(TemplateName, CONCAT(_instantiation_struct_instance_, __LINE__));                       \
  }

#define GET_FIRST(first, ...) first
#define GET_REST(first, ...) __VA_ARGS__

#define REGISTER_BENCHMARK_N(TemplateName, Index, PackedArgument)                                  \
                                                                                                   \
  namespace /* ensure internal linkage for struct */                                               \
  {                                                                                                \
  static CONCAT(TemplateName, BenchmarkParameters)                                                 \
      CONCAT(CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__),               \
             Index) = {GET_REST(PackedArgument)};                                                  \
  /* helper struct for static registration in ctor */                                              \
  struct CONCAT(TemplateName, CONCAT(CONCAT(_instantiation_struct_, __LINE__), Index)) {           \
    CONCAT(TemplateName, CONCAT(CONCAT(_instantiation_struct_, __LINE__),                          \
                                Index)()) { /* called once before main */                          \
      criterion::benchmark_template_registration_helper_struct::                                   \
          execute_registered_benchmark_template<CONCAT(TemplateName, BenchmarkParameters)>(        \
              #TemplateName, GET_FIRST(PackedArgument),                                            \
              CONCAT(CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__),       \
                     Index));                                                                      \
    }                                                                                              \
  } CONCAT(TemplateName, CONCAT(CONCAT(_instantiation_struct_instance_, __LINE__), Index));        \
  }

#define _Args(...) __VA_ARGS__
#define STRIP_PARENS(X) X
#define PASS_PARAMETERS(X) STRIP_PARENS(_Args X)

#define STRINGIZE(arg) STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2) CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2) CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2) arg1##arg2

#define FOR_EACH_1(what, first, x, ...) what(1, first, x);

#define FOR_EACH_2(what, first, x, ...)                                                            \
  what(2, first, x);                                                                               \
  FOR_EACH_1(what, first, __VA_ARGS__);

#define FOR_EACH_3(what, first, x, ...)                                                            \
  what(3, first, x);                                                                               \
  FOR_EACH_2(what, first, __VA_ARGS__);

#define FOR_EACH_4(what, first, x, ...)                                                            \
  what(4, first, x);                                                                               \
  FOR_EACH_3(what, first, __VA_ARGS__);

#define FOR_EACH_5(what, first, x, ...)                                                            \
  what(5, first, x);                                                                               \
  FOR_EACH_4(what, first, __VA_ARGS__);

#define FOR_EACH_6(what, first, x, ...)                                                            \
  what(6, first, x);                                                                               \
  FOR_EACH_5(what, first, __VA_ARGS__);

#define FOR_EACH_7(what, first, x, ...)                                                            \
  what(7, first, x);                                                                               \
  FOR_EACH_6(what, first, __VA_ARGS__);

#define FOR_EACH_8(what, first, x, ...)                                                            \
  what(8, first, x);                                                                               \
  FOR_EACH_7(what, first, __VA_ARGS__);

#define FOR_EACH_9(what, first, x, ...)                                                            \
  what(9, first, x);                                                                               \
  FOR_EACH_8(what, first, __VA_ARGS__);

#define FOR_EACH_10(what, first, x, ...)                                                           \
  what(10, first, x);                                                                              \
  FOR_EACH_9(what, first, __VA_ARGS__);

#define FOR_EACH_11(what, first, x, ...)                                                           \
  what(11, first, x);                                                                              \
  FOR_EACH_10(what, first, __VA_ARGS__);

#define FOR_EACH_12(what, first, x, ...)                                                           \
  what(12, first, x);                                                                              \
  FOR_EACH_11(what, first, __VA_ARGS__);

#define FOR_EACH_13(what, first, x, ...)                                                           \
  what(13, first, x);                                                                              \
  FOR_EACH_12(what, first, __VA_ARGS__);

#define FOR_EACH_14(what, first, x, ...)                                                           \
  what(14, first, x);                                                                              \
  FOR_EACH_13(what, first, __VA_ARGS__);

#define FOR_EACH_15(what, first, x, ...)                                                           \
  what(15, first, x);                                                                              \
  FOR_EACH_14(what, first, __VA_ARGS__);

#define FOR_EACH_16(what, first, x, ...)                                                           \
  what(16, first, x);                                                                              \
  FOR_EACH_15(what, first, __VA_ARGS__);

#define FOR_EACH_17(what, first, x, ...)                                                           \
  what(17, first, x);                                                                              \
  FOR_EACH_16(what, first, __VA_ARGS__);

#define FOR_EACH_18(what, first, x, ...)                                                           \
  what(18, first, x);                                                                              \
  FOR_EACH_17(what, first, __VA_ARGS__);

#define FOR_EACH_19(what, first, x, ...)                                                           \
  what(19, first, x);                                                                              \
  FOR_EACH_18(what, first, __VA_ARGS__);

#define FOR_EACH_20(what, first, x, ...)                                                           \
  what(20, first, x);                                                                              \
  FOR_EACH_19(what, first, __VA_ARGS__);

#define FOR_EACH_21(what, first, x, ...)                                                           \
  what(21, first, x);                                                                              \
  FOR_EACH_20(what, first, __VA_ARGS__);

#define FOR_EACH_22(what, first, x, ...)                                                           \
  what(22, first, x);                                                                              \
  FOR_EACH_21(what, first, __VA_ARGS__);

#define FOR_EACH_23(what, first, x, ...)                                                           \
  what(23, first, x);                                                                              \
  FOR_EACH_22(what, first, __VA_ARGS__);

#define FOR_EACH_24(what, first, x, ...)                                                           \
  what(24, first, x);                                                                              \
  FOR_EACH_23(what, first, __VA_ARGS__);

#define FOR_EACH_25(what, first, x, ...)                                                           \
  what(25, first, x);                                                                              \
  FOR_EACH_24(what, first, __VA_ARGS__);

#define FOR_EACH_26(what, first, x, ...)                                                           \
  what(26, first, x);                                                                              \
  FOR_EACH_25(what, first, __VA_ARGS__);

#define FOR_EACH_27(what, first, x, ...)                                                           \
  what(27, first, x);                                                                              \
  FOR_EACH_26(what, first, __VA_ARGS__);

#define FOR_EACH_28(what, first, x, ...)                                                           \
  what(28, first, x);                                                                              \
  FOR_EACH_27(what, first, __VA_ARGS__);

#define FOR_EACH_29(what, first, x, ...)                                                           \
  what(29, first, x);                                                                              \
  FOR_EACH_28(what, first, __VA_ARGS__);

#define FOR_EACH_30(what, first, x, ...)                                                           \
  what(30, first, x);                                                                              \
  FOR_EACH_29(what, first, __VA_ARGS__);

#define FOR_EACH_31(what, first, x, ...)                                                           \
  what(31, first, x);                                                                              \
  FOR_EACH_30(what, first, __VA_ARGS__);

#define FOR_EACH_32(what, first, x, ...)                                                           \
  what(32, first, x);                                                                              \
  FOR_EACH_31(what, first, __VA_ARGS__);

#define FOR_EACH_33(what, first, x, ...)                                                           \
  what(33, first, x);                                                                              \
  FOR_EACH_32(what, first, __VA_ARGS__);

#define FOR_EACH_34(what, first, x, ...)                                                           \
  what(34, first, x);                                                                              \
  FOR_EACH_33(what, first, __VA_ARGS__);

#define FOR_EACH_35(what, first, x, ...)                                                           \
  what(35, first, x);                                                                              \
  FOR_EACH_34(what, first, __VA_ARGS__);

#define FOR_EACH_36(what, first, x, ...)                                                           \
  what(36, first, x);                                                                              \
  FOR_EACH_35(what, first, __VA_ARGS__);

#define FOR_EACH_37(what, first, x, ...)                                                           \
  what(37, first, x);                                                                              \
  FOR_EACH_36(what, first, __VA_ARGS__);

#define FOR_EACH_38(what, first, x, ...)                                                           \
  what(38, first, x);                                                                              \
  FOR_EACH_37(what, first, __VA_ARGS__);

#define FOR_EACH_39(what, first, x, ...)                                                           \
  what(39, first, x);                                                                              \
  FOR_EACH_38(what, first, __VA_ARGS__);

#define FOR_EACH_40(what, first, x, ...)                                                           \
  what(40, first, x);                                                                              \
  FOR_EACH_39(what, first, __VA_ARGS__);

#define FOR_EACH_41(what, first, x, ...)                                                           \
  what(41, first, x);                                                                              \
  FOR_EACH_40(what, first, __VA_ARGS__);

#define FOR_EACH_42(what, first, x, ...)                                                           \
  what(42, first, x);                                                                              \
  FOR_EACH_41(what, first, __VA_ARGS__);

#define FOR_EACH_43(what, first, x, ...)                                                           \
  what(43, first, x);                                                                              \
  FOR_EACH_42(what, first, __VA_ARGS__);

#define FOR_EACH_44(what, first, x, ...)                                                           \
  what(44, first, x);                                                                              \
  FOR_EACH_43(what, first, __VA_ARGS__);

#define FOR_EACH_45(what, first, x, ...)                                                           \
  what(45, first, x);                                                                              \
  FOR_EACH_44(what, first, __VA_ARGS__);

#define FOR_EACH_46(what, first, x, ...)                                                           \
  what(46, first, x);                                                                              \
  FOR_EACH_45(what, first, __VA_ARGS__);

#define FOR_EACH_47(what, first, x, ...)                                                           \
  what(47, first, x);                                                                              \
  FOR_EACH_46(what, first, __VA_ARGS__);

#define FOR_EACH_48(what, first, x, ...)                                                           \
  what(48, first, x);                                                                              \
  FOR_EACH_47(what, first, __VA_ARGS__);

#define FOR_EACH_49(what, first, x, ...)                                                           \
  what(49, first, x);                                                                              \
  FOR_EACH_48(what, first, __VA_ARGS__);

#define FOR_EACH_50(what, first, x, ...)                                                           \
  what(50, first, x);                                                                              \
  FOR_EACH_49(what, first, __VA_ARGS__);

#define FOR_EACH_51(what, first, x, ...)                                                           \
  what(51, first, x);                                                                              \
  FOR_EACH_50(what, first, __VA_ARGS__);

#define FOR_EACH_52(what, first, x, ...)                                                           \
  what(52, first, x);                                                                              \
  FOR_EACH_51(what, first, __VA_ARGS__);

#define FOR_EACH_53(what, first, x, ...)                                                           \
  what(53, first, x);                                                                              \
  FOR_EACH_52(what, first, __VA_ARGS__);

#define FOR_EACH_54(what, first, x, ...)                                                           \
  what(54, first, x);                                                                              \
  FOR_EACH_53(what, first, __VA_ARGS__);

#define FOR_EACH_55(what, first, x, ...)                                                           \
  what(55, first, x);                                                                              \
  FOR_EACH_54(what, first, __VA_ARGS__);

#define FOR_EACH_56(what, first, x, ...)                                                           \
  what(56, first, x);                                                                              \
  FOR_EACH_55(what, first, __VA_ARGS__);

#define FOR_EACH_57(what, first, x, ...)                                                           \
  what(57, first, x);                                                                              \
  FOR_EACH_56(what, first, __VA_ARGS__);

#define FOR_EACH_58(what, first, x, ...)                                                           \
  what(58, first, x);                                                                              \
  FOR_EACH_57(what, first, __VA_ARGS__);

#define FOR_EACH_59(what, first, x, ...)                                                           \
  what(59, first, x);                                                                              \
  FOR_EACH_58(what, first, __VA_ARGS__);

#define FOR_EACH_60(what, first, x, ...)                                                           \
  what(60, first, x);                                                                              \
  FOR_EACH_59(what, first, __VA_ARGS__);

#define FOR_EACH_61(what, first, x, ...)                                                           \
  what(61, first, x);                                                                              \
  FOR_EACH_60(what, first, __VA_ARGS__);

#define FOR_EACH_62(what, first, x, ...)                                                           \
  what(62, first, x);                                                                              \
  FOR_EACH_61(what, first, __VA_ARGS__);

#define FOR_EACH_63(what, first, x, ...)                                                           \
  what(63, first, x);                                                                              \
  FOR_EACH_62(what, first, __VA_ARGS__);

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, \
                       _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32,  \
                       _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47,  \
                       _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,  \
                       _63, N, ...)                                                                \
  N
#define FOR_EACH_RSEQ_N()                                                                          \
  63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40,  \
      39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17,  \
      16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, first, x, ...) CONCATENATE(FOR_EACH_, N)(what, first, x, __VA_ARGS__)
#define FOR_EACH(what, first, x, ...)                                                              \
  FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, first, x, __VA_ARGS__)

#define REGISTER_BENCHMARK_FOR_EACH_HELPER(Index, TemplateName, ...)                               \
  REGISTER_BENCHMARK_N(TemplateName, Index, PASS_PARAMETERS(__VA_ARGS__))

#define REGISTER_BENCHMARK_FOR_EACH(TemplateName, ...)                                             \
  FOR_EACH(REGISTER_BENCHMARK_FOR_EACH_HELPER, TemplateName, __VA_ARGS__)