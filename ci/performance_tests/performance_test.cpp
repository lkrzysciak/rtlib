#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <deque>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#include "rtlib/vector.h"
#include "rtlib/list.h"
#include "rtlib/unordered_set.h"
#include "rtlib/set.h"
#include "rtlib/pool.h"
#include "rtlib/deque.h"
#include "rtlib/map.h"
#include "rtlib/unordered_map.h"

#include "rtlib/memory.h"

#define STATIC_CONTAINER_SIZE 100000

static int int_Compare(const int * v1, const int * v2)
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

unsigned int int_Hash(const int * value)
{
    return *value;
}

static_vector(TestVector, int, STATIC_CONTAINER_SIZE);
static_vector_impl(TestVector, int, STATIC_CONTAINER_SIZE);
static_list(TestList, int, STATIC_CONTAINER_SIZE);
static_list_impl(TestList, int, STATIC_CONTAINER_SIZE);
static_unordered_set(TestHashTable, int, STATIC_CONTAINER_SIZE);
static_unordered_set_impl(TestHashTable, int, STATIC_CONTAINER_SIZE);
static_set(TestBinaryTree, int, STATIC_CONTAINER_SIZE);
static_set_impl(TestBinaryTree, int, STATIC_CONTAINER_SIZE);
static_deque(TestDeque, int, STATIC_CONTAINER_SIZE);
static_deque_impl(TestDeque, int, STATIC_CONTAINER_SIZE);
static_map(TestMap, int, int, STATIC_CONTAINER_SIZE);
static_map_impl(TestMap, int, int, STATIC_CONTAINER_SIZE);
static_unordered_map(TestUnorderedMap, int, int, STATIC_CONTAINER_SIZE);
static_unordered_map_impl(TestUnorderedMap, int, int, STATIC_CONTAINER_SIZE);

dynamic_memory(DynamicAllocator);
dynamic_memory_impl(DynamicAllocator);
custom_allocator_vector(DynamicAllocatorVector, int, DynamicAllocator);
custom_allocator_vector_impl(DynamicAllocatorVector, int, DynamicAllocator);
custom_allocator_list(DynamicAllocatorList, int, DynamicAllocator);
custom_allocator_list_impl(DynamicAllocatorList, int, DynamicAllocator);
custom_allocator_unordered_set(DynamicAllocatorHashTable, int, DynamicAllocator);
custom_allocator_unordered_set_impl(DynamicAllocatorHashTable, int, DynamicAllocator);
custom_allocator_set(DynamicAllocatorBinaryTree, int, DynamicAllocator);
custom_allocator_set_impl(DynamicAllocatorBinaryTree, int, DynamicAllocator);
custom_allocator_map(DynamicAllocatorMap, int, int, DynamicAllocator);
custom_allocator_map_impl(DynamicAllocatorMap, int, int, DynamicAllocator);
custom_allocator_unordered_map(DynamicUnorderedMap, int, int, DynamicAllocator);
custom_allocator_unordered_map_impl(DynamicUnorderedMap, int, int, DynamicAllocator);

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
    rtlibType##_Construct(&rtlibObject);                                                 \
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

#define rtlibSetFind(rtlibType, oneIterationSize, iterations)                            \
    rtlibType rtlibObject;                                                               \
    rtlibType##_Construct(&rtlibObject);                                                 \
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
            volatile auto it = rtlibType##_Find(&rtlibObject, j);                        \
            (void)it;                                                                    \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define rtlibMapFind(rtlibType, oneIterationSize, iterations)                            \
    rtlibType rtlibObject;                                                               \
    rtlibType##_Construct(&rtlibObject);                                                 \
                                                                                         \
    for(int j = 0; j < oneIterationSize; ++j)                                            \
    {                                                                                    \
        rtlibType##_Insert(&rtlibObject, j, 0);                                          \
    }                                                                                    \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    for(int i = 0; i < iterations; ++i)                                                  \
    {                                                                                    \
        for(int j = 0; j < oneIterationSize; ++j)                                        \
        {                                                                                \
            volatile auto it = rtlibType##_Find(&rtlibObject, j);                        \
            (void)it;                                                                    \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define stlMapFind(stlType, oneIterationSize, iterations)                                \
    stlType stlObject;                                                                   \
                                                                                         \
    for(int j = 0; j < oneIterationSize; ++j)                                            \
    {                                                                                    \
        auto begin_it = std::begin(stlObject);                                           \
        stlObject.insert({ j, 0 });                                                      \
    }                                                                                    \
                                                                                         \
    auto start = std::chrono::high_resolution_clock::now();                              \
                                                                                         \
    for(int i = 0; i < iterations; ++i)                                                  \
    {                                                                                    \
        for(int j = 0; j < oneIterationSize; ++j)                                        \
        {                                                                                \
            volatile auto it = stlObject.find(j);                                        \
            (void)it;                                                                    \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define stlSetFind(stlType, oneIterationSize, iterations)                                \
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
            volatile auto it = stlObject.find(j);                                        \
            (void)it;                                                                    \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define rtlibTest(rtlibType, addMethod, deleteMethod, oneIterationSize, iterations)      \
    rtlibType rtlibObject;                                                               \
    rtlibType##_Construct(&rtlibObject);                                                 \
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
    rtlibType##_Construct(&rtlibObject);                                                    \
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
    rtlibType##_Construct(&rtlibObject);                                                    \
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
    rtlibType##_Construct(&object);                                                                       \
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
    for(int i = 0; i < iterations; ++i)                                                               \
    {                                                                                                 \
        for(int j = 0; j < oneIterationSize; ++j)                                                     \
        {                                                                                             \
            auto begin_it = std::begin(object);                                                       \
            for(int i = 0; i < position; ++i)                                                         \
            {                                                                                         \
                begin_it++;                                                                           \
            }                                                                                         \
            object.insert(begin_it, j);                                                               \
        }                                                                                             \
        for(int j = 0; j < oneIterationSize; ++j)                                                     \
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
    for(int i = 0; i < iterations; ++i)                                                         \
    {                                                                                           \
        for(int j = 0; j < oneIterationSize; ++j)                                               \
        {                                                                                       \
            object.insert(j);                                                                   \
        }                                                                                       \
        for(int j = 0; j < oneIterationSize; ++j)                                               \
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
unsigned int calculateRtlibStaticDequeBack()
{
    rtlibTest(TestDeque, TestDeque_PushBack, TestDeque_PopBack, onIterationSize, iterations);
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
unsigned int calculateSTLDequeBack()
{
    stlTest(std::deque<int>, push_back, pop_back, onIterationSize, iterations);
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
unsigned int calculateRtlibStaticDequeFront()
{
    rtlibTest(TestDeque, TestDeque_PushFront, TestDeque_PopFront, onIterationSize, iterations);
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

template<int onIterationSize, int iterations>
unsigned int calculateSTLDequeFront()
{
    stlTest(std::deque<int>, push_front, pop_front, onIterationSize, iterations);
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
unsigned int calculateRtlibStaticDequeMiddle()
{
    rtlibWithIteratorTest(TestDeque, Insert, Erase, onIterationSize, iterations, position);
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
unsigned int calculateSTLDequeMiddle()
{
    stlWithIteratorTest(std::deque<int>, insert, erase, onIterationSize, iterations, position);
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
unsigned int calculateRtlibStaticDequeFind()
{
    rtlibFind(TestDeque, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomVectorFind()
{
    rtlibFind(DynamicAllocatorVector, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticBinaryTreeFind()
{
    rtlibSetFind(TestBinaryTree, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomBinaryTreeFind()
{
    rtlibSetFind(DynamicAllocatorBinaryTree, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticHashFind()
{
    rtlibSetFind(TestHashTable, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomHashFind()
{
    rtlibSetFind(DynamicAllocatorHashTable, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibUnorderedMapFind()
{
    rtlibMapFind(TestUnorderedMap, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomUnorderedMapFind()
{
    rtlibMapFind(DynamicUnorderedMap, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibMapFind()
{
    rtlibMapFind(TestMap, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibCustomMapFind()
{
    rtlibMapFind(DynamicAllocatorMap, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlSetFind()
{
    stlSetFind(std::set<int>, onIterationSize, iterations);
}

using StdMap          = std::map<int, int>;
using StdUnorderedMap = std::unordered_map<int, int>;

template<int onIterationSize, int iterations>
unsigned int calculateStlMapFind()
{
    stlMapFind(StdMap, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlUnorderedSetFind()
{
    stlSetFind(std::unordered_set<int>, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateStlUnorderedMapFind()
{
    stlMapFind(StdUnorderedMap, onIterationSize, iterations);
}

static_pool(TestPool, int, 20);
static_pool_impl(TestPool, int, 20);

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

static constexpr int testedSizes[] = { 10, 20, 60, 100, 200, 600, 1000, 2000, 6000, 10000, 20000, 60000, 100000 };

void addRecordToTree2(boost::property_tree::ptree & array, std::string container, unsigned int x, unsigned int y)
{
    boost::property_tree::ptree child;

    child.put("container", container);
    child.put("x", x);
    child.put("y", y);

    array.push_back(std::make_pair("", child));

    std::cout << container << "(" << x << "): " << y << std::endl;
}

#define VECTOR_BACK_TEST(x, multiplier, output)                                                                     \
    addRecordToTree2(output, "rtlib static vector", x, calculateRtlibStaticVectorBack<x, multiplier>());            \
    addRecordToTree2(output, "rtlib dynamic vector", x, calculateRtlibDynamicAllocatorVectorBack<x, multiplier>()); \
    addRecordToTree2(output, "stl vector", x, calculateSTLVectorBack<x, multiplier>());

#define LIST_BACK_TEST(x, multiplier, output)                                                                   \
    addRecordToTree2(output, "rtlib static list", x, calculateRtlibStaticListBack<x, multiplier>());            \
    addRecordToTree2(output, "rtlib dynamic list", x, calculateRtlibDynamicAllocatorListBack<x, multiplier>()); \
    addRecordToTree2(output, "stl list", x, calculateSTLListBack<x, multiplier>());

#define LIST_FRONT_TEST(x, multiplier, output)                                                                   \
    addRecordToTree2(output, "rtlib static list", x, calculateRtlibStaticListFront<x, multiplier>());            \
    addRecordToTree2(output, "rtlib dynamic list", x, calculateRtlibDynamicAllocatorListFront<x, multiplier>()); \
    addRecordToTree2(output, "stl list", x, calculateSTLListFront<x, multiplier>());

#define LIST_MIDDLE_TEST(x, multiplier, output)                                                            \
    addRecordToTree2(output, "rtlib static list", x, calculateRtlibStaticListMiddle<x, multiplier, 1>());  \
    addRecordToTree2(output, "rtlib dynamic list", x, calculateRtlibCustomListMiddle<x, multiplier, 1>()); \
    addRecordToTree2(output, "stl list", x, calculateSTLListMiddle<x, multiplier, 1>());

#define DEQUE_BACK_TEST(x, multiplier, output)                                                         \
    addRecordToTree2(output, "rtlib static deque", x, calculateRtlibStaticDequeBack<x, multiplier>()); \
    addRecordToTree2(output, "stl deque", x, calculateSTLDequeBack<x, multiplier>());

#define DEQUE_FRONT_TEST(x, multiplier, output)                                                         \
    addRecordToTree2(output, "rtlib static deque", x, calculateRtlibStaticDequeFront<x, multiplier>()); \
    addRecordToTree2(output, "stl deque", x, calculateSTLDequeFront<x, multiplier>());

#define DEQUE_MIDDLE_TEST(x, multiplier, output)                                                            \
    addRecordToTree2(output, "rtlib static deque", x, calculateRtlibStaticDequeMiddle<x, multiplier, 1>()); \
    addRecordToTree2(output, "stl deque", x, calculateSTLDequeMiddle<x, multiplier, 1>());

#define SET_FIND_TEST(x, multiplier, output)                                                               \
    addRecordToTree2(output, "rtlib static set", x, calculateRtlibStaticBinaryTreeFind<x, multiplier>());  \
    addRecordToTree2(output, "rtlib dynamic set", x, calculateRtlibCustomBinaryTreeFind<x, multiplier>()); \
    addRecordToTree2(output, "stl set", x, calculateStlSetFind<x, multiplier>());

#define UNORDERED_SET_FIND_TEST(x, multiplier, output)                                                         \
    addRecordToTree2(output, "rtlib static unordered set", x, calculateRtlibStaticHashFind<x, multiplier>());  \
    addRecordToTree2(output, "rtlib dynamic unordered set", x, calculateRtlibCustomHashFind<x, multiplier>()); \
    addRecordToTree2(output, "stl unordered set", x, calculateStlUnorderedSetFind<x, multiplier>());

#define MAP_FIND_TEST(x, multiplier, output)                                                        \
    addRecordToTree2(output, "rtlib static map", x, calculateRtlibMapFind<x, multiplier>());        \
    addRecordToTree2(output, "rtlib dynamic map", x, calculateRtlibCustomMapFind<x, multiplier>()); \
    addRecordToTree2(output, "stl map", x, calculateStlMapFind<x, multiplier>());

#define UNORDERED_MAP_FIND_TEST(x, multiplier, output)                                                                 \
    addRecordToTree2(output, "rtlib static unordered map", x, calculateRtlibUnorderedMapFind<x, multiplier>());        \
    addRecordToTree2(output, "rtlib dynamic unordered map", x, calculateRtlibCustomUnorderedMapFind<x, multiplier>()); \
    addRecordToTree2(output, "stl unordered map", x, calculateStlUnorderedMapFind<x, multiplier>());

#define MAKE_10_SAMPLES(TEST, INIT, X, MULTIPLIER, TREE) \
    TEST(1 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(2 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(3 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(4 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(5 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(6 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(7 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(8 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(9 * X + INIT, MULTIPLIER, TREE)                 \
    TEST(10 * X + INIT, MULTIPLIER, TREE)

#define MAKE_100_SAMPLES(TEST, INIT, X, MULTIPLIER, TREE)     \
    MAKE_10_SAMPLES(TEST, INIT, X, MULTIPLIER, TREE)          \
    MAKE_10_SAMPLES(TEST, 10 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 20 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 30 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 40 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 50 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 60 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 70 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 80 * X + INIT, X, MULTIPLIER, TREE) \
    MAKE_10_SAMPLES(TEST, 90 * X + INIT, X, MULTIPLIER, TREE)

#define MAKE_SUITE(TEST, MULTIPLIER, OUTPUT)                         \
    boost::property_tree::ptree OUTPUT##_100{};                      \
    MAKE_100_SAMPLES(TEST, 0, 1, MULTIPLIER, OUTPUT##_100)           \
    generateFile(OUTPUT##_100, #OUTPUT "_100.json");                 \
                                                                     \
    boost::property_tree::ptree OUTPUT##_1000{};                     \
    MAKE_100_SAMPLES(TEST, 0, 10, MULTIPLIER / 10, OUTPUT##_1000)    \
    generateFile(OUTPUT##_1000, #OUTPUT "_1000.json");               \
                                                                     \
    boost::property_tree::ptree OUTPUT##_10000{};                    \
    MAKE_100_SAMPLES(TEST, 0, 100, MULTIPLIER / 100, OUTPUT##_10000) \
    generateFile(OUTPUT##_10000, #OUTPUT "_10000.json");

static void runQueueTests()
{
    MAKE_SUITE(VECTOR_BACK_TEST, 300000, vec_back);
    MAKE_SUITE(LIST_BACK_TEST, 30000, list_back);
    MAKE_SUITE(LIST_FRONT_TEST, 30000, list_front);
    MAKE_SUITE(LIST_MIDDLE_TEST, 30000, list_middle);
    MAKE_SUITE(DEQUE_BACK_TEST, 300000, deque_back);
    MAKE_SUITE(DEQUE_FRONT_TEST, 300000, deque_front);
    MAKE_SUITE(DEQUE_MIDDLE_TEST, 10000, deque_middle);
}

static void runSetTests()
{
    MAKE_SUITE(SET_FIND_TEST, 100000, set_find);
    MAKE_SUITE(UNORDERED_SET_FIND_TEST, 100000, unordered_set_find);
}

static void runMapTests()
{
    MAKE_SUITE(MAP_FIND_TEST, 100000, map_find);
    MAKE_SUITE(UNORDERED_MAP_FIND_TEST, 100000, unordered_map_find);
}

int main()
{
    runQueueTests();
    runSetTests();
    runMapTests();
    return 0;
}