cmake_minimum_required(VERSION 3.13)

project(rtlib-ut)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
  gt
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
  SOURCE_DIR     ${CMAKE_CURRENT_LIST_DIR}/gt
)

FetchContent_Populate(gt)

add_subdirectory(${gt_SOURCE_DIR})

set(TESTS
    pool_test.cpp
    queue_containers_test.cpp
    set_test.cpp
    map_test.cpp
    string_test.cpp
    nested_test.cpp
    )

add_executable(ut ut.cpp ${TESTS})
target_link_libraries(ut gtest gtest_main gmock gmock_main rtlib)
target_compile_options(ut PUBLIC -g -Wall -Werror)

add_executable(performance_tests_Os ${TESTED_FILES} performance_tests/performance_test.cpp)
target_link_libraries(performance_tests_Os rtlib)
target_compile_options(performance_tests_Os PUBLIC -Os)
target_compile_definitions(performance_tests_Os PUBLIC -DNDEBUG)

add_executable(c_dec_def c_tests/dec_def.c c_tests/def.c)
target_link_libraries(c_dec_def rtlib)
