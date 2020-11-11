cmake_minimum_required(VERSION 3.0)

project(rtlib)

message(STATUS "Building ${CMAKE_BUILD_TYPE} version")

add_subdirectory(src)
add_subdirectory(ci)