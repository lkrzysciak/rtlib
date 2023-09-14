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
    )

add_executable(ut ut.cpp ${TESTS})
target_link_libraries(ut gtest gtest_main gmock gmock_main rtlib)

add_executable(performance_tests_Os ${TESTED_FILES} performance_tests/performance_test.cpp)
target_link_libraries(performance_tests_Os rtlib)
target_compile_options(performance_tests_Os PUBLIC -Os)
target_compile_definitions(performance_tests_Os PUBLIC -DNDEBUG)

add_executable(c_static_vector c_tests/static_vector.c)
target_link_libraries(c_static_vector rtlib)

add_executable(c_static_list c_tests/static_list.c)
target_link_libraries(c_static_list rtlib)

add_executable(c_static_binary_tree c_tests/static_binary_tree.c)
target_link_libraries(c_static_binary_tree rtlib)

add_executable(c_static_hash_table c_tests/static_hash_table.c)
target_link_libraries(c_static_hash_table rtlib)

add_executable(c_static_deque c_tests/static_deque.c)
target_link_libraries(c_static_deque rtlib)

add_executable(c_custom_vector c_tests/custom_vector.c)
target_link_libraries(c_custom_vector rtlib)

add_executable(c_custom_list c_tests/custom_list.c)
target_link_libraries(c_custom_list rtlib)

add_executable(c_custom_binary_tree c_tests/custom_binary_tree.c)
target_link_libraries(c_custom_binary_tree rtlib)

add_executable(c_custom_hash_table c_tests/custom_hash_table.c)
target_link_libraries(c_custom_hash_table rtlib)

add_executable(c_dynamic_vector c_tests/dynamic_vector.c)
target_link_libraries(c_dynamic_vector rtlib)

add_executable(c_dynamic_list c_tests/dynamic_list.c)
target_link_libraries(c_dynamic_list rtlib)

add_executable(c_dynamic_binary_tree c_tests/dynamic_binary_tree.c)
target_link_libraries(c_dynamic_binary_tree rtlib)

add_executable(c_dynamic_hash_table c_tests/dynamic_hash_table.c)
target_link_libraries(c_dynamic_hash_table rtlib)

add_executable(c_dec_def c_tests/dec_def.c c_tests/def.c)
target_link_libraries(c_dec_def rtlib)
