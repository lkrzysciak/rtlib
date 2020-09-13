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

typed_vector_t(TestVector, int, 20);
typed_list_t(TestList, int, 20);

void calculateRtlibStaticListBack()
{
    TestList typed_list; 
    TestList_Init(&typed_list);

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
}

void calculateRtlibStaticVectorBack()
{
    TestVector vector; 
    TestVector_Init(&vector);

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
}

void calculateSTLListBack()
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
}

void calculateSTLVectorBack()
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
}

void calculateRtlibGenericStaticListBack_ForTest()
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
}

void calculateRtlibGenericStaticVectorBack_ForTest()
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
}

void calculateOneDirectMemoryOperation_ForTest()
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
}

void calculateMultipleDirectMemoryOperation_ForTest()
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
}


void calculateRtlibStaticListFront()
{
    TestList typed_list; 
    TestList_Init(&typed_list);

    
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
}

void calculateRtlibStaticVectorFront()
{
    TestVector vector; 
    TestVector_Init(&vector);

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
}

void calculateSTLListFront()
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
}

void calculateRtlibStaticListMiddle()
{
    TestList typed_list; 
    TestList_Init(&typed_list);

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
}

void calculateRtlibStaticVectorMiddle()
{
    TestVector vector; 
    TestVector_Init(&vector);

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
}

void calculateSTLListMiddle()
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
}

typed_pool_t(TestPool, int, 20);

void measureGenericPool()
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
}

void measureTypedPool()
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
}

int main()
{
    /* Back */
    std::cout << "Back: " << std::endl;
    calculateRtlibStaticListBack();
    calculateRtlibStaticVectorBack();
    calculateSTLListBack();
    calculateSTLVectorBack();

    /* Front */
    std::cout << "Front: " << std::endl;
    calculateRtlibStaticListFront();
    calculateRtlibStaticVectorFront();
    calculateSTLListFront();

    /* Middle */
    std::cout << "Middle: " << std::endl;
    calculateRtlibStaticListMiddle();
    calculateRtlibStaticVectorMiddle();
    calculateSTLListMiddle();

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