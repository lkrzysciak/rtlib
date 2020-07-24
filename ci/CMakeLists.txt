cmake_minimum_required(VERSION 3.15)

project(rtlib-ut)

include_directories(googletest-release-1.10.0/googletest/include)
include_directories(${CMAKE_HOME_DIRECTORY}/src)

add_subdirectory(googletest-release-1.10.0)

set(TESTS
    list_test.cpp
    heap_test.cpp
    object_pool_test.cpp)

set(PERF_TESTS
    performance_tests/perf_test.cpp)

add_executable(ut ut.cpp ${TESTS})
target_link_libraries(ut gtest gtest_main gmock gmock_main rtlib)

add_executable(perf ${PERF_TESTS})
target_link_libraries(perf rtlib)