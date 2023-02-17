#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "for_tests/list.h"
#include "for_tests/vector.h"
#include "for_tests/object_pool.h"

#include "rtlib/vector.h"
#include "rtlib/list.h"
#include "rtlib/hash_table.h"
#include "rtlib/binary_tree.h"
#include "rtlib/pool.h"

#include "rtlib/memory.h"

vector_t(TestVector, int);
static_vector_t(TestVector, int, 2000);
list_t(TestList, int);
static_list_t(TestList, int, 2000);
hash_table_t(TestHashTable, int);
static_hash_table_t(TestHashTable, int, 2000);
binary_tree_t(TestBinaryTree, int);
static_binary_tree_t(TestBinaryTree, int, 2000);

memory_t(DynamicAllocator);
dynamic_memory_t(DynamicAllocator);
vector_t(DynamicAllocatorVector, int);
custom_allocator_vector_t(DynamicAllocatorVector, int, DynamicAllocator);
list_t(DynamicAllocatorList, int);
custom_allocator_list_t(DynamicAllocatorList, int, DynamicAllocator);
hash_table_t(DynamicAllocatorHashTable, int);
custom_allocator_hash_table_t(DynamicAllocatorHashTable, int, DynamicAllocator);
binary_tree_t(DynamicAllocatorBinaryTree, int);
custom_allocator_binary_tree_t(DynamicAllocatorBinaryTree, int, DynamicAllocator);

static int compare_set_ints(const int * v1, const int * v2)
{
    if(*v1 > *v2)
    {
        return 1;
    }
    else if(*v1 < *v2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

unsigned int hash_function(const int * value)
{
    return *value;
}

#define cCall(object, addMethod, deleteMethod, oneIterationSize, iterations) \
    for(int i = 0; i < iterations; ++i)                                      \
    {                                                                        \
        for(int j = 0; j < oneIterationSize; ++j)                            \
        {                                                                    \
            addMethod(&object, j);                                           \
        }                                                                    \
        for(int j = 0; j < oneIterationSize; ++j)                            \
        {                                                                    \
            deleteMethod(&object);                                           \
        }                                                                    \
    }

#define cppCall(object, addMethod, deleteMethod, oneIterationSize, iterations) \
    for(int i = 0; i < iterations; ++i)                                        \
    {                                                                          \
        for(int j = 0; j < oneIterationSize; ++j)                              \
        {                                                                      \
            object.addMethod(j);                                               \
        }                                                                      \
        for(int j = 0; j < oneIterationSize; ++j)                              \
        {                                                                      \
            object.deleteMethod();                                             \
        }                                                                      \
    }

/* Adding to rtlib container without iterators */
#define rtlibFind(rtlibType, oneIterationSize, iterations)                               \
    rtlibType rtlibObject;                                                               \
    rtlibType##_Construct(&rtlibObject, compare_set_ints);                               \
                                                                                         \
    for(int j = 0; j < oneIterationSize; ++j)                                            \
    {                                                                                    \
        auto it = rtlibType##_Begin(&rtlibObject);                                       \
        rtlibType##_Insert(&rtlibObject, &it, j);                                        \
    }                                                                                    \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    for(int i = 0; i < iterations; ++i)                                                  \
    {                                                                                    \
        for(int j = 0; j < oneIterationSize; ++j)                                        \
        {                                                                                \
            rtlibType##_Find(&rtlibObject, j);                                           \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define rtlibBinaryTreeFind(rtlibType, oneIterationSize, iterations)                     \
    rtlibType rtlibObject;                                                               \
    rtlibType##_Construct(&rtlibObject, compare_set_ints);                               \
                                                                                         \
    for(int j = 0; j < oneIterationSize; ++j)                                            \
    {                                                                                    \
        rtlibType##_Insert(&rtlibObject, j);                                             \
    }                                                                                    \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    for(int i = 0; i < iterations; ++i)                                                  \
    {                                                                                    \
        for(int j = 0; j < oneIterationSize; ++j)                                        \
        {                                                                                \
            rtlibType##_Find(&rtlibObject, j);                                           \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define rtlibHashTableFind(rtlibType, oneIterationSize, iterations)                      \
    rtlibType rtlibObject;                                                               \
    rtlibType##_Construct(&rtlibObject, compare_set_ints, hash_function);                \
                                                                                         \
    for(int j = 0; j < oneIterationSize; ++j)                                            \
    {                                                                                    \
        rtlibType##_Insert(&rtlibObject, j);                                             \
    }                                                                                    \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    for(int i = 0; i < iterations; ++i)                                                  \
    {                                                                                    \
        for(int j = 0; j < oneIterationSize; ++j)                                        \
        {                                                                                \
            rtlibType##_Find(&rtlibObject, j);                                           \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define stlFind(stlType, oneIterationSize, iterations)                                   \
    stlType stlObject;                                                                   \
                                                                                         \
    for(int j = 0; j < oneIterationSize; ++j)                                            \
    {                                                                                    \
        auto begin_it = std::begin(stlObject);                                           \
        stlObject.insert(begin_it, j);                                                   \
    }                                                                                    \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    for(int i = 0; i < iterations; ++i)                                                  \
    {                                                                                    \
        for(int j = 0; j < oneIterationSize; ++j)                                        \
        {                                                                                \
            std::find(std::begin(stlObject), std::end(stlObject), j);                    \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define rtlibTest(rtlibType, addMethod, deleteMethod, oneIterationSize, iterations)      \
    rtlibType rtlibObject;                                                               \
    rtlibType##_Construct(&rtlibObject, compare_set_ints);                               \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    cCall(rtlibObject, addMethod, deleteMethod, oneIterationSize, iterations);           \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

/* Adding to rtlib container without iterators */
#define rtlibTestWithHash(rtlibType, addMethod, deleteMethod, oneIterationSize, iterations) \
    rtlibType rtlibObject;                                                                  \
    rtlibType##_Construct(&rtlibObject, compare_set_ints, hash_function);                   \
                                                                                            \
    auto start = std::chrono::high_resolution_clock::now();                                 \
                                                                                            \
    for(int i = 0; i < iterations; ++i)                                                     \
    {                                                                                       \
        for(int j = 0; j < oneIterationSize; ++j)                                           \
        {                                                                                   \
            addMethod(&rtlibObject, j);                                                     \
        }                                                                                   \
        for(int j = 0; j < oneIterationSize; ++j)                                           \
        {                                                                                   \
            auto it = rtlibType##_Begin(&rtlibObject);                                      \
            deleteMethod(&rtlibObject, &it);                                                \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    auto stop     = std::chrono::high_resolution_clock::now();                              \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);    \
    return duration.count();

#define rtlibTestWithTree(rtlibType, addMethod, deleteMethod, oneIterationSize, iterations) \
    rtlibType rtlibObject;                                                                  \
    rtlibType##_Construct(&rtlibObject, compare_set_ints);                                  \
                                                                                            \
    auto start = std::chrono::high_resolution_clock::now();                                 \
                                                                                            \
    for(int i = 0; i < iterations; ++i)                                                     \
    {                                                                                       \
        for(int j = 0; j < oneIterationSize; ++j)                                           \
        {                                                                                   \
            addMethod(&rtlibObject, j);                                                     \
        }                                                                                   \
        for(int j = 0; j < oneIterationSize; ++j)                                           \
        {                                                                                   \
            auto it = rtlibType##_Begin(&rtlibObject);                                      \
            deleteMethod(&rtlibObject, &it);                                                \
        }                                                                                   \
    }                                                                                       \
                                                                                            \
    auto stop     = std::chrono::high_resolution_clock::now();                              \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);    \
    return duration.count();

/* Adding to stl container with iterators */
#define rtlibWithIteratorTest(rtlibType, addMethod, deleteMethod, oneIterationSize, iterations, position) \
    rtlibType object;                                                                                     \
    rtlibType##_Construct(&object, compare_set_ints);                                                     \
    for(int i = 0; i < position; ++i)                                                                     \
    {                                                                                                     \
        auto begin_it = rtlibType##_Begin(&object);                                                       \
        rtlibType##_Insert(&object, &begin_it, 0);                                                        \
    }                                                                                                     \
                                                                                                          \
    auto start = std::chrono::high_resolution_clock::now();                                               \
                                                                                                          \
    for(int i = 0; i < iterations; ++i)                                                                   \
    {                                                                                                     \
        for(int j = 0; j < oneIterationSize; ++j)                                                         \
        {                                                                                                 \
            auto it = rtlibType##_Begin(&object);                                                         \
            for(int i = 0; i < position; ++i)                                                             \
            {                                                                                             \
                rtlibType##_Iterator_Increment(&it);                                                      \
            }                                                                                             \
            rtlibType##_##addMethod(&object, &it, j);                                                     \
        }                                                                                                 \
        for(int j = 0; j < oneIterationSize; ++j)                                                         \
        {                                                                                                 \
            auto it = rtlibType##_Begin(&object);                                                         \
            for(int i = 0; i < position; ++i)                                                             \
            {                                                                                             \
                rtlibType##_Iterator_Increment(&it);                                                      \
            }                                                                                             \
            rtlibType##_##deleteMethod(&object, &it);                                                     \
        }                                                                                                 \
    }                                                                                                     \
    auto stop     = std::chrono::high_resolution_clock::now();                                            \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);                  \
    return duration.count();

/* Adding to stl container without iterators */
#define stlTest(stlType, addMethod, deleteMethod, oneIterationSize, iterations)          \
    stlType stlObject;                                                                   \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    cppCall(stlObject, addMethod, deleteMethod, oneIterationSize, iterations);           \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

/* Adding to stl container with iterators */
#define stlWithIteratorTest(stlType, addMethod, deleteMethod, oneIterationSize, iterations, position) \
    stlType object{};                                                                                 \
    for(int i = 0; i < position; ++i)                                                                 \
    {                                                                                                 \
        auto begin_it = std ::begin(object);                                                          \
        object.insert(begin_it, 0);                                                                   \
    }                                                                                                 \
    auto start = std::chrono::high_resolution_clock::now();                                           \
                                                                                                      \
    for(int i = 0; i < 10000000; ++i)                                                                 \
    {                                                                                                 \
        for(int j = 0; j < 16; ++j)                                                                   \
        {                                                                                             \
            auto begin_it = std::begin(object);                                                       \
            for(int i = 0; i < position; ++i)                                                         \
            {                                                                                         \
                begin_it++;                                                                           \
            }                                                                                         \
            object.insert(begin_it, j);                                                               \
        }                                                                                             \
        for(int j = 0; j < 16; ++j)                                                                   \
        {                                                                                             \
            auto begin_it = std::begin(object);                                                       \
            for(int i = 0; i < position; ++i)                                                         \
            {                                                                                         \
                begin_it++;                                                                           \
            }                                                                                         \
            object.erase(begin_it);                                                                   \
        }                                                                                             \
    }                                                                                                 \
    auto stop     = std::chrono::high_resolution_clock::now();                                        \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);              \
    return duration.count();

/* Adding to stl container with iterators */
#define stlNoQueueContainerTest(stlType, addMethod, deleteMethod, oneIterationSize, iterations) \
    stlType object{};                                                                           \
    auto start = std::chrono::high_resolution_clock::now();                                     \
                                                                                                \
    for(int i = 0; i < 10000000; ++i)                                                           \
    {                                                                                           \
        for(int j = 0; j < 16; ++j)                                                             \
        {                                                                                       \
            object.insert(j);                                                                   \
        }                                                                                       \
        for(int j = 0; j < 16; ++j)                                                             \
        {                                                                                       \
            auto begin_it = std::begin(object);                                                 \
            object.erase(begin_it);                                                             \
        }                                                                                       \
    }                                                                                           \
    auto stop     = std::chrono::high_resolution_clock::now();                                  \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);        \
    return duration.count();

/* Tests */
template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticListBack()
{
    rtlibTest(TestList, TestList_PushBack, TestList_PopBack, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibDynamicAllocatorListBack()
{
    rtlibTest(DynamicAllocatorList, DynamicAllocatorList_PushBack, DynamicAllocatorList_PopBack, onIterationSize,
              iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticVectorBack()
{
    rtlibTest(TestVector, TestVector_PushBack, TestVector_PopBack, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibDynamicAllocatorVectorBack()
{
    rtlibTest(DynamicAllocatorVector, DynamicAllocatorVector_PushBack, DynamicAllocatorVector_PopBack, onIterationSize,
              iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateSTLListBack()
{
    stlTest(std::list<int>, push_back, pop_back, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateSTLVectorBack()
{
    stlTest(std::vector<int>, push_back, pop_back, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticListFront()
{
    rtlibTest(TestList, TestList_PushFront, TestList_PopFront, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibDynamicAllocatorListFront()
{
    rtlibTest(DynamicAllocatorList, DynamicAllocatorList_PushFront, DynamicAllocatorList_PopFront, onIterationSize,
              iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticVectorFront()
{
    rtlibTest(TestVector, TestVector_PushFront, TestVector_PopFront, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibDynamicAllocatorVectorFront()
{
    rtlibTest(DynamicAllocatorVector, DynamicAllocatorVector_PushFront, DynamicAllocatorVector_PopFront,
              onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateSTLListFront()
{
    stlTest(std::list<int>, push_front, pop_front, onIterationSize, iterations);
}

template<int onIterationSize, int iterations, int position>
unsigned int calculateRtlibStaticListMiddle()
{
    rtlibWithIteratorTest(TestList, Insert, Erase, onIterationSize, iterations, position);
}

template<int onIterationSize, int iterations, int position>
unsigned int calculateRtlibCustomListMiddle()
{
    rtlibWithIteratorTest(DynamicAllocatorList, Insert, Erase, onIterationSize, iterations, position);
}

template<int onIterationSize, int iterations, int position>
unsigned int calculateRtlibStaticVectorMiddle()
{
    rtlibWithIteratorTest(TestVector, Insert, Erase, onIterationSize, iterations, position);
}

template<int onIterationSize, int iterations, int position>
unsigned int calculateRtlibCustomVectorMiddle()
{
    rtlibWithIteratorTest(DynamicAllocatorVector, Insert, Erase, onIterationSize, iterations, position);
}

template<int onIterationSize, int iterations, int position>
unsigned int calculateSTLListMiddle()
{
    stlWithIteratorTest(std::list<int>, insert, erase, onIterationSize, iterations, position);
}

template<int onIterationSize, int iterations, int position>
unsigned int calculateSTLVectorMiddle()
{
    stlWithIteratorTest(std::vector<int>, insert, erase, onIterationSize, iterations, position);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticHashTable()
{
    rtlibTestWithHash(TestHashTable, TestHashTable_Insert, TestHashTable_Erase, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticBinaryTree()
{
    rtlibTestWithTree(TestBinaryTree, TestBinaryTree_Insert, TestBinaryTree_Erase, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomHashTable()
{
    rtlibTestWithHash(DynamicAllocatorHashTable, DynamicAllocatorHashTable_Insert, DynamicAllocatorHashTable_Erase,
                      onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomBinaryTree()
{
    rtlibTestWithTree(DynamicAllocatorBinaryTree, DynamicAllocatorBinaryTree_Insert, DynamicAllocatorBinaryTree_Erase,
                      onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlSet()
{
    stlNoQueueContainerTest(std::set<int>, insert, erase, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlUnorderedSet()
{
    stlNoQueueContainerTest(std::unordered_set<int>, insert, erase, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticListFind()
{
    rtlibFind(TestList, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomListFind()
{
    rtlibFind(DynamicAllocatorList, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticVectorFind()
{
    rtlibFind(TestVector, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomVectorFind()
{
    rtlibFind(DynamicAllocatorVector, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticBinaryTreeFind()
{
    rtlibBinaryTreeFind(TestBinaryTree, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomBinaryTreeFind()
{
    rtlibBinaryTreeFind(DynamicAllocatorBinaryTree, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticHashFind()
{
    rtlibHashTableFind(TestHashTable, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomHashFind()
{
    rtlibHashTableFind(DynamicAllocatorHashTable, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlListFind()
{
    stlFind(std::list<int>, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlVectorFind()
{
    stlFind(std::vector<int>, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlSetFind()
{
    stlFind(std::set<int>, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlUnorderedSetFind()
{
    stlFind(std::unordered_set<int>, onIterationSize, iterations);
}

pool_t(TestPool, int);
static_pool_t(TestPool, int, 20);

unsigned int measureGenericPool()
{
    uint32_t pool_buf[10000];
    Pool * pool = Pool_Init(pool_buf, sizeof(pool_buf), sizeof(int));

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 10000000; ++i)
    {
        for(int j = 0; j < 16; ++j)
        {
            void * ptr = Pool_Alloc(pool);
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
    TestPool_Construct(&test_pool);

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 10000000; ++i)
    {
        for(int j = 0; j < 16; ++j)
        {
            int * ptr = TestPool_Allocate(&test_pool);
            TestPool_Release(&test_pool, ptr);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[RTLib][pool-typed-static]: " << duration.count() << std::endl;
    return duration.count();
}

unsigned int calculateRtlibGenericStaticListBack_ForTest()
{
    uint32_t buf1[10000];
    List * vector = List_Init(buf1, sizeof(buf1), sizeof(int));

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 10000000; ++i)
    {
        for(int j = 0; j < 16; ++j)
        {
            List_PushBack(vector, &j);
        }
        for(int j = 0; j < 16; ++j)
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

    for(int i = 0; i < 10000000; ++i)
    {
        for(int j = 0; j < 16; ++j)
        {
            Vector_PushBack(vector, &j);
        }
        for(int j = 0; j < 16; ++j)
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
    int samples[16]{};

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 10000000; ++i)
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
    int samples[16]{};

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 10000000; ++i)
    {
        for(int j = 0; j < 16; ++j)
        {
            memcpy(temp_buf, samples, sizeof(temp_buf));
        }
        for(int j = 0; j < 16; ++j)
        {
            memset(temp_buf, 0, sizeof(temp_buf));
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[memory][c][multiple]: " << duration.count() << std::endl;
    return duration.count();
}

void addRecordToTree(boost::property_tree::ptree & array, std::string container, unsigned int duration)
{
    boost::property_tree::ptree child;

    child.put("container", container);
    child.put("duration", duration);

    array.push_back(std::make_pair("", child));

    std::cout << container << ": " << duration << std::endl;
}

void generateFile(const boost::property_tree::ptree & array, std::string filename)
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
    addRecordToTree(backTree, "rtlib-svector", calculateRtlibStaticVectorBack<16, 10000000>());
    addRecordToTree(backTree, "rtlib-cvector", calculateRtlibDynamicAllocatorVectorBack<16, 10000000>());
    addRecordToTree(backTree, "rtlib-slist", calculateRtlibStaticListBack<16, 10000000>());
    addRecordToTree(backTree, "rtlib-clist", calculateRtlibDynamicAllocatorListBack<16, 10000000>());
    addRecordToTree(backTree, "stl-vector", calculateSTLVectorBack<16, 10000000>());
    addRecordToTree(backTree, "stl-list", calculateSTLListBack<16, 10000000>());
    generateFile(backTree, "back.json");

    /* Front */
    boost::property_tree::ptree frontTree{};
    std::cout << "Front: " << std::endl;
    addRecordToTree(frontTree, "rtlib-svector", calculateRtlibStaticVectorFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-cvector", calculateRtlibDynamicAllocatorVectorFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-slist", calculateRtlibStaticListFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-clist", calculateRtlibDynamicAllocatorListFront<16, 10000000>());
    // STL vector has no push_front method
    addRecordToTree(frontTree, "stl-list", calculateSTLListFront<16, 10000000>());
    generateFile(frontTree, "front.json");

    /* Middle */
    boost::property_tree::ptree middleTree{};
    std::cout << "Middle: " << std::endl;
    addRecordToTree(middleTree, "rtlib-svector", calculateRtlibStaticVectorMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "rtlib-cvector", calculateRtlibCustomVectorMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "rtlib-slist", calculateRtlibStaticListMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "rtlib-clist", calculateRtlibCustomListMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "stl-vector", calculateSTLVectorMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "stl-list", calculateSTLListMiddle<16, 10000000, 1>());
    generateFile(middleTree, "middle.json");

    boost::property_tree::ptree noQueueTree{};
    std::cout << "No queue container: " << std::endl;
    addRecordToTree(noQueueTree, "rtlib-shashtable", calculateRtlibStaticHashTable<16, 10000000>());
    addRecordToTree(noQueueTree, "rtlib-chashtable", calculateRtlibCustomHashTable<16, 10000000>());
    addRecordToTree(noQueueTree, "rtlib-sbinarytree", calculateRtlibStaticBinaryTree<16, 10000000>());
    addRecordToTree(noQueueTree, "rtlib-cbinarytree", calculateRtlibCustomBinaryTree<16, 10000000>());
    addRecordToTree(noQueueTree, "stl-set", calculateStlSet<16, 10000000>());
    addRecordToTree(noQueueTree, "stl-unorderedset", calculateStlUnorderedSet<16, 10000000>());
    generateFile(noQueueTree, "non-queue-tree.json");

    boost::property_tree::ptree find16{};
    std::cout << "Find - 16: " << std::endl;
    addRecordToTree(find16, "rtlib-svector", calculateRtlibStaticVectorFind<16, 10000000>());
    addRecordToTree(find16, "rtlib-cvector", calculateRtlibCustomVectorFind<16, 10000000>());
    addRecordToTree(find16, "rtlib-slist", calculateRtlibStaticListFind<16, 10000000>());
    addRecordToTree(find16, "rtlib-clist", calculateRtlibCustomListFind<16, 10000000>());
    addRecordToTree(find16, "stl-vector", calculateStlVectorFind<16, 10000000>());
    addRecordToTree(find16, "stl-list", calculateStlListFind<16, 10000000>());
    addRecordToTree(find16, "rtlib-shashtable", calculateRtlibStaticHashFind<16, 10000000>());
    addRecordToTree(find16, "rtlib-chashtable", calculateRtlibCustomHashFind<16, 10000000>());
    addRecordToTree(find16, "rtlib-sbinarytree", calculateRtlibStaticBinaryTreeFind<16, 10000000>());
    addRecordToTree(find16, "rtlib-cbinarytree", calculateRtlibCustomBinaryTreeFind<16, 10000000>());
    addRecordToTree(find16, "stl-set", calculateStlSetFind<16, 10000000>());
    addRecordToTree(find16, "stl-unorderedset", calculateStlUnorderedSetFind<16, 10000000>());
    generateFile(find16, "find-16.json");

    boost::property_tree::ptree find1024{};
    std::cout << "Find - 1024: " << std::endl;
    addRecordToTree(find1024, "rtlib-svector", calculateRtlibStaticVectorFind<1024, 10000>());
    addRecordToTree(find1024, "rtlib-cvector", calculateRtlibCustomVectorFind<1024, 10000>());
    addRecordToTree(find1024, "rtlib-slist", calculateRtlibStaticListFind<1024, 10000>());
    addRecordToTree(find1024, "rtlib-clist", calculateRtlibCustomListFind<1024, 10000>());
    addRecordToTree(find1024, "stl-vector", calculateStlVectorFind<1024, 10000>());
    addRecordToTree(find1024, "stl-list", calculateStlListFind<1024, 10000>());
    addRecordToTree(find1024, "rtlib-shashtable", calculateRtlibStaticHashFind<1024, 10000>());
    addRecordToTree(find1024, "rtlib-chashtable", calculateRtlibCustomHashFind<1024, 10000>());
    addRecordToTree(find1024, "rtlib-sbinarytree", calculateRtlibStaticBinaryTreeFind<1024, 10000>());
    addRecordToTree(find1024, "rtlib-cbinarytree", calculateRtlibCustomBinaryTreeFind<1024, 10000>());
    addRecordToTree(find1024, "stl-set", calculateStlSetFind<1024, 10000>());
    addRecordToTree(find1024, "stl-unorderedset", calculateStlUnorderedSetFind<1024, 10000>());
    generateFile(find1024, "find-1024.json");

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