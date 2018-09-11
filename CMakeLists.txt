cmake_minimum_required(VERSION 3.4.0)
project(cpp-template-meta)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/compiler_options.cmake)

add_subdirectory(boost-cmake)
add_subdirectory(src)

