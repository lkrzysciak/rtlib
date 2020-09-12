#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include "containers/list.h"
#include "containers/vector.h"
#include <memory_resource>
#include "containers/typed_vector.h"
#include "containers/typed_list.h"

int samples[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

#define calculate_time_c_value_arg(container_ob, add, remove, begin_fun, next_it_fun, description) \
{ \
auto begin_it = begin_fun(container_ob); \
add(container_ob, &begin_it, 0); \
begin_it = begin_fun(container_ob); \
add(container_ob, &begin_it, 0); \
\
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        begin_it = begin_fun(container_ob); \
        auto next_it = next_it_fun(&begin_it); \
        add(container_ob, &next_it, samples[j]); \
    } \
    for(int j=0; j<16; ++j) \
    { \
        begin_it = begin_fun(container_ob); \
        auto next_it = next_it_fun(&begin_it); \
        remove(container_ob, &next_it); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
\
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}

#define calculate_time_cpp_value_arg(container_ob, add, remove, description) \
{ \
auto begin_it = std::begin(container_ob); \
container_ob.add(begin_it, 0); \
begin_it = std::begin(container_ob); \
container_ob.add(begin_it, 0); \
\
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        begin_it = std::begin(container_ob); \
        begin_it++; \
        container_ob.add(begin_it, samples[j]); \
    } \
    for(int j=0; j<16; ++j) \
    { \
        begin_it = std::begin(container_ob); \
        begin_it++; \
        container_ob.remove(begin_it); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
\
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}

typed_vector_t(TestVector, int, 20);
typed_list_t(TestList, int, 20);

int main()
{
    TestList typed_list; 
    TestList_Init(&typed_list);
    calculate_time_c_value_arg(&typed_list, TestList_Insert, TestList_Erase, TestList_Begin, TestList_Iterator_Next, "[list][rtlib][typed][middle]: ");

    TestVector typed_vector; 
    TestVector_Init(&typed_vector);
    calculate_time_c_value_arg(&typed_vector, TestVector_Insert, TestVector_Erase, TestVector_Begin, TestVector_Iterator_Next, "[vector][rtlib][typed][middle]: ");

    std::list<int> temp_list{};
    calculate_time_cpp_value_arg(temp_list, insert, erase, "[list][stl][middle]: ");

    return 0;
}