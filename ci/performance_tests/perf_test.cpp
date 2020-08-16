#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include "containers/list.h"
#include "containers/vector.h"
#include <memory_resource>
#include "containers/typed_vector.h"

int samples[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

typed_vector_t(TestType, int);

#define calculate_time_c_ptr_arg(container_ob, push_back_ptr, pop_back_ptr, description) \
{ \
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        push_back_ptr(container_ob, &samples[j]); \
    } \
    for(int j=0; j<16; ++j) \
    { \
        pop_back_ptr(container_ob); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
\
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}

#define calculate_time_c_value_arg(container_ob, push_back_ptr, pop_back_ptr, description) \
{ \
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        push_back_ptr(container_ob, samples[j]); \
    } \
    for(int j=0; j<16; ++j) \
    { \
        pop_back_ptr(container_ob); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
\
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}

#define calculate_time_cpp_value_arg(container_ob, push_back_ptr, pop_back_ptr, description) \
{ \
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        container_ob.push_back_ptr(samples[j]); \
    } \
    for(int j=0; j<16; ++j) \
    { \
        container_ob.pop_back_ptr(); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
\
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}


int main()
{
    uint32_t buf[10000];
    List * list = List_Init(buf, sizeof(buf), sizeof(int));

    calculate_time_c_ptr_arg(list, List_PushBack, List_PopBack, "[list][rtlib][generic]: ");


    std::list<int> temp_list{};
    calculate_time_cpp_value_arg(temp_list, push_back, pop_back, "[list][stl]: ");


    uint32_t buf1[10000];
    Vector * vector = Vector_Init(buf1, sizeof(buf1), sizeof(int));
    calculate_time_c_ptr_arg(vector, Vector_PushBack, Vector_PopBack, "[vector][rtlib][generic]: ");

    /****************************************************/
    
    uint32_t buf2[10000];
    TestType_Vector * vector_tt = TestType_Vector_Init(buf2, sizeof(buf2));
    calculate_time_c_value_arg(vector_tt, TestType_Vector_PushBack, TestType_Vector_PopBack, "[vector][rtlib][typed]: ");

    std::vector<int> temp_vector{};
    calculate_time_cpp_value_arg(temp_vector, push_back, pop_back, "[vector][stl]: ");
    
    return 0;
}