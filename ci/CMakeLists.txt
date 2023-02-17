cmake_minimum_required(VERSION 3.0)

project(rtlib-ut)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_STANDARD 11)

include_directories(googletest-release-1.10.0/googletest/include)
include_directories(${CMAKE_HOME_DIRECTORY}/src)
include_directories(.)

add_subdirectory(googletest-release-1.10.0)

set(TESTED_FILES
    for_tests/list.c
    for_tests/vector.c
    for_tests/object_pool.c)

set(TESTS
    pool_test.cpp
    queue_containers_test.cpp
    set_test.cpp
    )

set(CMAKE_CXX_FLAGS "-Wall -Werror -save-temps=obj")

add_library(tested_utils ${TESTED_FILES})

add_executable(ut ut.cpp ${TESTS})
target_link_libraries(ut gtest gtest_main gmock gmock_main tested_utils rtlib)

add_executable(performance_tests_O0 ${TESTED_FILES} performance_tests/performance_test.cpp)
target_link_libraries(performance_tests_O0 tested_utils rtlib)
target_compile_options(performance_tests_O0 PUBLIC -O0)

add_executable(performance_tests_O3 ${TESTED_FILES} performance_tests/performance_test.cpp)
target_link_libraries(performance_tests_O3 tested_utils rtlib)
target_compile_options(performance_tests_O3 PUBLIC -O3)

add_executable(c_static_vector c_tests/static_vector.c)
target_link_libraries(c_static_vector rtlib)

add_executable(c_static_list c_tests/static_list.c)
target_link_libraries(c_static_list rtlib)

add_executable(c_static_binary_tree c_tests/static_binary_tree.c)
target_link_libraries(c_static_binary_tree rtlib)

add_executable(c_static_hash_table c_tests/static_hash_table.c)
target_link_libraries(c_static_hash_table rtlib)

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

