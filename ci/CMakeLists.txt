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

add_compile_options(-Wall)

add_executable(ut ut.cpp ${TESTS} ${TESTED_FILES})
target_link_libraries(ut gtest gtest_main gmock gmock_main)

# add_executable(pool_O0 ${TESTED_FILES} performance_tests/pool_performance_test.cpp)
add_executable(pool_O3 ${TESTED_FILES} performance_tests/pool_performance_test.cpp)
# target_compile_options(pool_O0 PRIVATE -O0 -save-temps)
target_compile_options(pool_O3 PRIVATE -O3 )

# add_executable(front_O0 ${TESTED_FILES} performance_tests/front_performance_test.cpp)
add_executable(front_O3 ${TESTED_FILES} performance_tests/front_performance_test.cpp)
# target_compile_options(front_O0 PRIVATE -O0 -save-temps)
target_compile_options(front_O3 PRIVATE -O3)

# add_executable(back_O0 ${TESTED_FILES} performance_tests/back_performance_test.cpp)
add_executable(back_O3 ${TESTED_FILES} performance_tests/back_performance_test.cpp)
# target_compile_options(back_O0 PRIVATE -O0 -save-temps)
target_compile_options(back_O3 PRIVATE -O3)

# add_executable(middle_O0 ${TESTED_FILES} performance_tests/middle_performance_test.cpp)
add_executable(middle_O3 ${TESTED_FILES} performance_tests/middle_performance_test.cpp)
# target_compile_options(middle_O0 PRIVATE -O0)
target_compile_options(middle_O3 PRIVATE -O3)

