# this script adds all subprojects to a single build to allow IDEs understand the full project
# structure.

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(BuildAll LANGUAGES CXX)

include(../cmake/tools.cmake)

# set(LIBRARY_NAME benchmark)
# add_library(${LIBRARY_NAME} ${CMAKE_CURRENT_LIST_DIR}/../source/benchmark.cpp)
# target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)
# set_property(TARGET ${LIBRARY_NAME} PROPERTY CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../samples ${CMAKE_BINARY_DIR}/samples)
