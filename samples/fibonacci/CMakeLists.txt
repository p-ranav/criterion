cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Fibonacci LANGUAGES CXX)

# --- Import tools ----

include(../../cmake/tools.cmake)

# ---- Dependencies ----

include(../../cmake/CPM.cmake)

CPMAddPackage(NAME Criterion SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE Release)
# endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(Fibonacci ${sources})
target_include_directories(Fibonacci PRIVATE ${Criterion_INCLUDE_DIRS})
set_target_properties(Fibonacci PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "fibonacci")
target_link_libraries(Fibonacci Criterion)