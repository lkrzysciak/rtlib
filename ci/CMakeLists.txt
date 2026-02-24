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

set(RTLIB_WARN_FLAGS -Wall -Werror)

set(TESTS
    pool_test.cpp
    queue_containers_test.cpp
  heap_test.cpp
    set_test.cpp
    map_test.cpp
    string_test.cpp
    nested_test.cpp
    )

add_executable(ut ut.cpp ${TESTS})
target_link_libraries(ut gtest gtest_main gmock gmock_main rtlib)
target_compile_options(ut PUBLIC -g ${RTLIB_WARN_FLAGS})

add_library(perf_common OBJECT performance_tests/perf_common.cpp)
target_link_libraries(perf_common rtlib)
target_compile_options(perf_common PUBLIC -Os ${RTLIB_WARN_FLAGS})
target_compile_definitions(perf_common PUBLIC -DNDEBUG)
target_compile_options(perf_common PUBLIC -flto)
target_link_options(perf_common PUBLIC -flto)

add_library(perf_suites OBJECT
  performance_tests/perf_vec_back.cpp
  performance_tests/perf_list_back.cpp
  performance_tests/perf_list_front.cpp
  performance_tests/perf_list_middle.cpp
  performance_tests/perf_deque_back.cpp
  performance_tests/perf_deque_front.cpp
  performance_tests/perf_deque_middle.cpp
  performance_tests/perf_set_find.cpp
  performance_tests/perf_unordered_set_find.cpp
  performance_tests/perf_map_find.cpp
  performance_tests/perf_unordered_map_find.cpp
  performance_tests/perf_heap_malloc_free.cpp
  performance_tests/perf_heap_calloc_free.cpp
  performance_tests/perf_heap_realloc_free.cpp
)
target_link_libraries(perf_suites rtlib)
target_compile_options(perf_suites PUBLIC -Os ${RTLIB_WARN_FLAGS})
target_compile_definitions(perf_suites PUBLIC -DNDEBUG)
target_compile_options(perf_suites PUBLIC -flto)
target_link_options(perf_suites PUBLIC -flto)

add_executable(
  performance_tests_Os
  ${TESTED_FILES}
  performance_tests/performance_test.cpp
  $<TARGET_OBJECTS:perf_common>
  $<TARGET_OBJECTS:perf_suites>
)
target_link_libraries(performance_tests_Os rtlib)
target_compile_options(performance_tests_Os PUBLIC -Os ${RTLIB_WARN_FLAGS})
target_compile_definitions(performance_tests_Os PUBLIC -DNDEBUG)
target_compile_options(performance_tests_Os PUBLIC -flto)
target_link_options(performance_tests_Os PUBLIC -flto)

add_executable(c_dec_def c_tests/dec_def.c c_tests/def.c)
target_link_libraries(c_dec_def rtlib)
target_compile_options(c_dec_def PUBLIC ${RTLIB_WARN_FLAGS})

add_executable(c_comparator_uint8 c_tests/comparator_uint8.c)
target_link_libraries(c_comparator_uint8 rtlib)
target_compile_options(c_comparator_uint8 PUBLIC -Wall -Wextra -Werror)

add_executable(c_hash_uint8 c_tests/hash_uint8.c)
target_link_libraries(c_hash_uint8 rtlib)
target_compile_options(c_hash_uint8 PUBLIC -Wall -Wextra -Werror)
