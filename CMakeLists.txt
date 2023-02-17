cmake_minimum_required(VERSION 3.0)

project(rtlib)

message(STATUS "Building ${CMAKE_BUILD_TYPE} version")

add_library(
    rtlib
    INTERFACE
)

target_include_directories(
    rtlib
    INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(ci)