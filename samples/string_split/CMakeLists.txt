cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(StringSplit LANGUAGES CXX)

# --- Import tools ----

include(../../cmake/tools.cmake)

# ---- Dependencies ----

include(../../cmake/CPM.cmake)

CPMAddPackage(NAME Benchmark SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(StringSplit ${sources})
target_include_directories(StringSplit PRIVATE ${Benchmark_INCLUDE_DIRS})
set_target_properties(StringSplit PROPERTIES CXX_STANDARD 11 OUTPUT_NAME "string_split")
target_link_libraries(StringSplit Benchmark)