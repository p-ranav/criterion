cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

function(add_sample name)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${name} ${CMAKE_BINARY_DIR}/samples/${name})
endfunction()

add_sample(vector_sort)
add_sample(string_split)
add_sample(make_shared)
# add_sample(parameterized)