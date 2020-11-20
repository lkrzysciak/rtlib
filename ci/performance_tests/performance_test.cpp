#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "for_tests/list.h"
#include "for_tests/vector.h"
#include "for_tests/object_pool.h"
#include "containers/static_vector.h"
#include "containers/static_list.h"
#include "memory/static_one_chunk_allocator.h"
#include "memory/dynamic_allocator.h"
#include "containers/custom_allocator_vector.h"
#include "memory/typed_pool.h"

int samples[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

declare_static_vector_t(TestVector, int, 20);
define_static_vector_t(TestVector, int, 20);

declare_static_list_t(TestList, int, 20);
define_static_list_t(TestList, int, 20);

declare_static_one_chunk_allocator_t(StaticOneChunkllocator, 20 * sizeof(int));
define_static_one_chunk_allocator_t(StaticOneChunkllocator, 20 * sizeof(int));
declare_custom_allocator_vector_t(CustomAllocatorVector, int, StaticOneChunkllocator);
define_custom_allocator_vector_t(CustomAllocatorVector, int, StaticOneChunkllocator);

declare_dynamic_allocator_t(DynamicAllocator);
define_dynamic_allocator_t(DynamicAllocator);
declare_custom_allocator_vector_t(DynamicAllocatorVector, int, DynamicAllocator);
define_custom_allocator_vector_t(DynamicAllocatorVector, int, DynamicAllocator);

unsigned int calculateRtlibStaticListBack()
{
    TestList typed_list; 
    TestList_Construct(&typed_list);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            TestList_PushBack(&typed_list, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            TestList_PopBack(&typed_list);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][list-typed-static][back]: " << duration.count() << std::endl;

    return duration.count();
}

unsigned int calculateRtlibStaticVectorBack()
{
    TestVector vector; 
    TestVector_Construct(&vector);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            TestVector_PushBack(&vector, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            TestVector_PopBack(&vector);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][vector-typed-static][back]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibStaticCustomAllocatorVectorBack()
{
    CustomAllocatorVector vector; 
    CustomAllocatorVector_Construct(&vector);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            CustomAllocatorVector_PushBack(&vector, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            CustomAllocatorVector_PopBack(&vector);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][vector-typed-custom(static)][back]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibDynamicAllocatorVectorBack()
{
    DynamicAllocatorVector vector; 
    DynamicAllocatorVector_Construct(&vector);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            DynamicAllocatorVector_PushBack(&vector, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            DynamicAllocatorVector_PopBack(&vector);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][vector-typed-custom(dynamic)][back]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateSTLListBack()
{
    std::list<int> temp_list{};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i) 
    {
        for(int j=0; j<16; ++j)
        {
            temp_list.push_back(samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            temp_list.pop_back();
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[STL][list][back]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateSTLVectorBack()
{
    std::vector<int> temp_vector{};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i) 
    {
        for(int j=0; j<16; ++j)
        {
            temp_vector.push_back(samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            temp_vector.pop_back();
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[STL][vector][back]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibGenericStaticListBack_ForTest()
{
    uint32_t buf1[10000];
    List * vector = List_Init(buf1, sizeof(buf1), sizeof(int));

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            List_PushBack(vector, &samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            List_PopBack(vector);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][list-generic-static][back]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibGenericStaticVectorBack_ForTest()
{
    uint32_t buf1[10000];
    Vector * vector = Vector_Init(buf1, sizeof(buf1), sizeof(int));

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            Vector_PushBack(vector, &samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            Vector_PopBack(vector);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][vector-generic-static][back]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateOneDirectMemoryOperation_ForTest()
{
    int temp_buf[16]{};

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        memcpy(temp_buf, samples, sizeof(temp_buf));
        memset(temp_buf, 0, sizeof(temp_buf));
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[memory][c][once]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateMultipleDirectMemoryOperation_ForTest()
{
    int temp_buf[16]{};

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            memcpy(temp_buf, samples, sizeof(temp_buf));
        }
        for(int j=0; j<16; ++j)
        {
            memset(temp_buf, 0, sizeof(temp_buf));
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[memory][c][multiple]: " << duration.count() << std::endl;
    return duration.count();
}


unsigned int calculateRtlibStaticListFront()
{
    TestList typed_list; 
    TestList_Construct(&typed_list);

    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            TestList_PushFront(&typed_list, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            TestList_PopFront(&typed_list);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][list-typed-static][front]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibStaticVectorFront()
{
    TestVector vector; 
    TestVector_Construct(&vector);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            TestVector_PushFront(&vector, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            TestVector_PopFront(&vector);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][vector-typed-static][front]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateSTLListFront()
{
    std::list<int> temp_list{};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i) 
    {
        for(int j=0; j<16; ++j)
        {
            temp_list.push_front(samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            temp_list.pop_front();
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[STL][list][front]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibStaticListMiddle()
{
    TestList typed_list; 
    TestList_Construct(&typed_list);

    auto begin_it = TestList_Begin(&typed_list);
    TestList_Insert(&typed_list, &begin_it, 0);
    begin_it = TestList_Begin(&typed_list);
    TestList_Insert(&typed_list, &begin_it, 0);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            auto it = TestList_Begin(&typed_list);
            TestList_Iterator_Increment(&it);
            TestList_Insert(&typed_list, &it, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            auto it = TestList_Begin(&typed_list);
            TestList_Iterator_Increment(&it);
            TestList_Erase(&typed_list, &it);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][list-typed-static][middle]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibStaticVectorMiddle()
{
    TestVector vector; 
    TestVector_Construct(&vector);

    auto begin_it = TestVector_Begin(&vector);
    TestVector_Insert(&vector, &begin_it, 0);
    begin_it = TestVector_Begin(&vector);
    TestVector_Insert(&vector, &begin_it, 0);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            auto it = TestVector_Begin(&vector);
            TestVector_Iterator_Increment(&it);
            TestVector_Insert(&vector, &it, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            auto it = TestVector_Begin(&vector);
            TestVector_Iterator_Increment(&it);
            TestVector_Erase(&vector, &it);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][vector-typed-static][middle]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateSTLListMiddle()
{
    std::list<int> temp_list{};
    auto begin_it = std::begin(temp_list);
    temp_list.insert(begin_it, 0);
    begin_it = std::begin(temp_list);
    temp_list.insert(begin_it, 0);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i) 
    {
        for(int j=0; j<16; ++j)
        {
            begin_it = std::begin(temp_list);
            begin_it++;
            temp_list.insert(begin_it, samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            begin_it = std::begin(temp_list);
            begin_it++;
            temp_list.erase(begin_it);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[STL][list][middle]: " << duration.count() << std::endl;
    return duration.count();
}

typed_pool_t(TestPool, int, 20);

unsigned int measureGenericPool()
{
    uint32_t pool_buf[10000];
    Pool* pool = Pool_Init(pool_buf, sizeof(pool_buf), sizeof(int));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            void* ptr = Pool_Alloc(pool);
            Pool_Free(pool, ptr);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][pool-generic-static]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int measureTypedPool()
{
    TestPool test_pool{};
    TestPool_Init(&test_pool);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            int* ptr = TestPool_Alloc(&test_pool);
            TestPool_Free(&test_pool, ptr);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][pool-typed-static]: " << duration.count() << std::endl;
    return duration.count();
}

void addRecordToTree(boost::property_tree::ptree& array, std::string container, unsigned int duration)
{
    boost::property_tree::ptree child;

    child.put("container", container);
    child.put("duration", duration);

    array.push_back(std::make_pair("", child));
}

void generateFile(const boost::property_tree::ptree& array, std::string filename)
{
    boost::property_tree::ptree pt;

    pt.add_child("data", array);

    boost::property_tree::write_json(filename, pt);
}

int main()
{
    boost::property_tree::ptree backTree{};
    /* Back */
    std::cout << "Back: " << std::endl;
    addRecordToTree(backTree, "rtlib-svector", calculateRtlibStaticVectorBack());
    addRecordToTree(backTree, "rtlib-cvector", calculateRtlibDynamicAllocatorVectorBack());
    addRecordToTree(backTree, "rtlib-slist", calculateRtlibStaticListBack());
    addRecordToTree(backTree, "stl-vector", calculateSTLVectorBack());
    addRecordToTree(backTree, "stl-list", calculateSTLListBack());
    generateFile(backTree, "back.json");

    /* Front */
    boost::property_tree::ptree frontTree{};
    std::cout << "Front: " << std::endl;
    addRecordToTree(frontTree, "rtlib-static-list",calculateRtlibStaticListFront());
    addRecordToTree(frontTree, "rtlib-static-vector",calculateRtlibStaticVectorFront());
    addRecordToTree(frontTree, "stl-list",calculateSTLListFront());
    generateFile(backTree, "front.json");

    /* Middle */
    boost::property_tree::ptree middleTree{};
    std::cout << "Middle: " << std::endl;
    addRecordToTree(middleTree, "rtlib-static-list" ,calculateRtlibStaticListMiddle());
    addRecordToTree(middleTree, "rtlib-static-vector",calculateRtlibStaticVectorMiddle());
    addRecordToTree(middleTree, "stl-list", calculateSTLListMiddle());
    generateFile(middleTree, "middle.json");

    /* Pool */
    std::cout << "Pool: " << std::endl;
    measureGenericPool();
    measureTypedPool();

    /* For test */
    std::cout << "For tests: " << std::endl;
    calculateRtlibGenericStaticListBack_ForTest();
    calculateRtlibGenericStaticVectorBack_ForTest();
    calculateOneDirectMemoryOperation_ForTest();
    calculateMultipleDirectMemoryOperation_ForTest();

    return 0;
}