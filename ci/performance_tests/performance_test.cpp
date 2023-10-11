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

#define rtlibBinaryTreeFind(rtlibType, oneIterationSize, iterations)                     \
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
            rtlibType##_Find(&rtlibObject, j);                                           \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    auto stop     = std::chrono::high_resolution_clock::now();                           \
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
    return duration.count();

#define rtlibHashTableFind(rtlibType, oneIterationSize, iterations)                      \
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

int main()
{
    MAKE_SUITE(VECTOR_BACK_TEST, 300000, vec_back);
    MAKE_SUITE(LIST_BACK_TEST, 30000, list_back);
    MAKE_SUITE(LIST_FRONT_TEST, 30000, list_front);
    MAKE_SUITE(LIST_MIDDLE_TEST, 30000, list_middle);
    MAKE_SUITE(DEQUE_BACK_TEST, 100000, deque_back);
    MAKE_SUITE(DEQUE_FRONT_TEST, 100000, deque_front);
    MAKE_SUITE(DEQUE_MIDDLE_TEST, 30000, deque_middle);

    /* Front */
    boost::property_tree::ptree frontTree{};
    std::cout << "Front: " << std::endl;
    addRecordToTree(frontTree, "rtlib-svector", calculateRtlibStaticVectorFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-cvector", calculateRtlibDynamicAllocatorVectorFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-slist", calculateRtlibStaticListFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-clist", calculateRtlibDynamicAllocatorListFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-sdeque", calculateRtlibStaticDequeFront<16, 10000000>());
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
    addRecordToTree(middleTree, "rtlib-sdeque", calculateRtlibStaticDequeMiddle<16, 10000000, 1>());
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
    addRecordToTree(find16, "rtlib-sdeque", calculateRtlibStaticDequeFind<16, 10000000>());
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
    addRecordToTree(find16, "rtlib-sdeque", calculateRtlibStaticDequeFind<1024, 10000>());
    addRecordToTree(find1024, "stl-set", calculateStlSetFind<1024, 10000>());
    addRecordToTree(find1024, "stl-unorderedset", calculateStlUnorderedSetFind<1024, 10000>());
    generateFile(find1024, "find-1024.json");

    return 0;
}