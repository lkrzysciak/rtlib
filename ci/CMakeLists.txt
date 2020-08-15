cmake_minimum_required(VERSION 3.15)

project(rtlib-ut)

set (CMAKE_CXX_STANDARD 17)

include_directories(googletest-release-1.10.0/googletest/include)
include_directories(${CMAKE_HOME_DIRECTORY}/src)

add_subdirectory(googletest-release-1.10.0)

set(CMAKE_CXX_FLAGS "-Wall -Werror -O3")
set(CMAKE_C_FLAGS "-Wall -Werror -O3")

set(TESTS
    list_test.cpp
    vector_test.cpp
    heap_test.cpp
    object_pool_test.cpp)

set(PERF_TESTS
    performance_tests/perf_test.cpp)

add_executable(ut ut.cpp ${TESTS})
target_link_libraries(ut gtest gtest_main gmock gmock_main rtlib)

add_executable(perf ${PERF_TESTS})
target_link_libraries(perf rtlib)