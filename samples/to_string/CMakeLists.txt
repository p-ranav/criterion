cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(ToString LANGUAGES CXX)

# --- Import tools ----

include(../../cmake/tools.cmake)

# ---- Dependencies ----

include(../../cmake/CPM.cmake)

CPMAddPackage(NAME Criterion SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(ToString ${sources})
target_include_directories(ToString PRIVATE ${Criterion_INCLUDE_DIRS})
set_target_properties(ToString PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "to_string")
target_link_libraries(ToString Criterion)