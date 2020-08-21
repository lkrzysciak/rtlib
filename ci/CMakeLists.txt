cmake_minimum_required(VERSION 3.0)

project(rtlib-ut)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_STANDARD 11)

include_directories(googletest-release-1.10.0/googletest/include)
include_directories(${CMAKE_HOME_DIRECTORY}/src)

add_subdirectory(googletest-release-1.10.0)

set(TESTED_FILES
    ../src/containers/list.c
    ../src/containers/vector.c
    ../src/memory/object_pool.c)

set(TESTS
    list_test.cpp
    vector_test.cpp
    heap_test.cpp
    object_pool_test.cpp
    typed_pool_test.cpp
    typed_containers_test.cpp
    )

set(PERF_TESTS
    performance_tests/perf_test.cpp)

add_executable(ut ut.cpp ${TESTS} ${TESTED_FILES})
target_link_libraries(ut gtest gtest_main gmock gmock_main)

add_executable(perf_O0 ${PERF_TESTS} ${TESTED_FILES})
add_executable(perf_O3 ${PERF_TESTS} ${TESTED_FILES})

target_compile_options(perf_O0 PRIVATE "-O0")
target_compile_options(perf_O3 PRIVATE "-O3")
add_compile_options("-Wall -Werror -g")
