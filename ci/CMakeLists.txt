cmake_minimum_required(VERSION 3.15)

project(rtlib-ut)

include_directories(googletest-release-1.10.0/googletest/include)

add_subdirectory(googletest-release-1.10.0)

add_executable(ut ut.cpp)
target_link_libraries(ut gtest gtest_main gmock gmock_main)