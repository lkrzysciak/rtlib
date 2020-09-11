#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include "containers/list.h"
#include "containers/vector.h"
#include <memory_resource>
#include "containers/typed_vector.h"
#include "containers/typed_list.h"
#include "memory/object_pool.h"
#include "memory/typed_pool.h"

int samples[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

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
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
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
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
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
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}


#define calculate_pool_alloc_time(pool_ob, alloc_ptr, dealloc_ptr, description) \
{ \
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        void* ptr = alloc_ptr(pool_ob); \
        dealloc_ptr(pool_ob, ptr); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
\
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}


#define calculate_typed_pool_alloc_time(pool_ob, alloc_ptr, dealloc_ptr, description, type) \
{ \
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        type* ptr = alloc_ptr(pool_ob); \
        dealloc_ptr(pool_ob, ptr); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
\
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
std::cout << description << duration.count() << std::endl; \
}

typed_vector_t(TestVector, int, 20);
typed_list_t(TestList, int, 20);
typed_pool_t(TestPool, int, 20);

int main()
{
    uint32_t pool_buf[10000];
    Pool* pool = Pool_Init(pool_buf, sizeof(pool_buf), sizeof(int));
    calculate_pool_alloc_time(pool, Pool_Alloc, Pool_Free, "[pool][rtlib][generic]: ");

    TestPool test_pool{};
    TestPool_Init(&test_pool);
    calculate_typed_pool_alloc_time(&test_pool, TestPool_Alloc, TestPool_Free, "[pool][rtlib][typed]: ", int);

    uint32_t buf[10000];
    List * list = List_Init(buf, sizeof(buf), sizeof(int));
    calculate_time_c_ptr_arg(list, List_PushBack, List_PopBack, "[list][rtlib][generic][back]: ");

    TestList test_list; 
    TestList_Init(&test_list);
    calculate_time_c_value_arg(&test_list, TestList_PushBack, TestList_PopBack, "[list][rtlib][typed][back]: ");

    std::list<int> temp_list{};
    calculate_time_cpp_value_arg(temp_list, push_back, pop_back, "[list][stl][back]: ");

    uint32_t buf1[10000];
    Vector * vector = Vector_Init(buf1, sizeof(buf1), sizeof(int));
    calculate_time_c_ptr_arg(vector, Vector_PushBack, Vector_PopBack, "[vector][rtlib][generic][back]: ");

    TestVector vector_tt; 
    TestVector_Init(&vector_tt);
    calculate_time_c_value_arg(&vector_tt, TestVector_PushBack, TestVector_PopBack, "[vector][rtlib][typed][back]: ");

    std::vector<int> temp_vector{};
    calculate_time_cpp_value_arg(temp_vector, emplace_back, pop_back, "[vector][stl][back]: ");

    TestList typed_list_2; 
    TestList_Init(&typed_list_2);
    calculate_time_c_value_arg(&typed_list_2, TestList_PushFront, TestList_PopFront, "[list][rtlib][typed][front]: ");

    TestVector typed_vector_2; 
    TestVector_Init(&typed_vector_2);
    calculate_time_c_value_arg(&typed_vector_2, TestVector_PushFront, TestVector_PopFront, "[vector][rtlib][typed][front]: ");

    return 0;
}