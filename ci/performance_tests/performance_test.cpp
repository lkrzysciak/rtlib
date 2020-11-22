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
#include "containers/custom_allocator_list.h"
#include "memory/typed_pool.h"

declare_static_vector_t(TestVector, int, 20);
define_static_vector_t(TestVector, int, 20);
declare_static_list_t(TestList, int, 20);
define_static_list_t(TestList, int, 20);

declare_dynamic_allocator_t(DynamicAllocator);
define_dynamic_allocator_t(DynamicAllocator);
declare_custom_allocator_vector_t(DynamicAllocatorVector, int, DynamicAllocator);
define_custom_allocator_vector_t(DynamicAllocatorVector, int, DynamicAllocator);
declare_custom_allocator_list_t(DynamicAllocatorList, int, DynamicAllocator);
define_custom_allocator_list_t(DynamicAllocatorList, int, DynamicAllocator);



#define cCall(object, addMethod, deleteMethod, oneIterationSize, iterations) \
for(int i=0; i<iterations; ++i) \
{ \
    for(int j=0; j<oneIterationSize; ++j) \
    { \
        addMethod(&object, j); \
    } \
    for(int j=0; j<oneIterationSize; ++j) \
    { \
        deleteMethod(&object); \
    } \
}

#define cppCall(object, addMethod, deleteMethod, oneIterationSize, iterations) \
for(int i=0; i<iterations; ++i) \
{ \
    for(int j=0; j<oneIterationSize; ++j) \
    { \
        object.addMethod(j); \
    } \
    for(int j=0; j<oneIterationSize; ++j) \
    { \
        object.deleteMethod(); \
    } \
}

/* Adding to rtlib container without iterators */
#define rtlibTest(rtlibType, addMethod, deleteMethod, oneIterationSize, iterations) \
rtlibType rtlibObject; \
rtlibType##_Construct(&rtlibObject); \
\
auto start = std::chrono::high_resolution_clock::now(); \
\
cCall(rtlibObject, addMethod, deleteMethod, oneIterationSize, iterations); \
\
auto stop = std::chrono::high_resolution_clock::now(); \
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
return duration.count();


/* Adding to stl container with iterators */
#define rtlibWithIteratorTest(rtlibType, addMethod, deleteMethod, oneIterationSize, iterations, position) \
rtlibType object; \
rtlibType##_Construct(&object); \
for(int i=0; i<position; ++i) \
{ \
    auto begin_it = rtlibType##_Begin(&object); \
    rtlibType##_Insert(&object, &begin_it, 0); \
} \
\
auto start = std::chrono::high_resolution_clock::now(); \
 \
for(int i=0; i<iterations; ++i) \
{ \
    for(int j=0; j<oneIterationSize; ++j) \
    { \
        auto it = rtlibType##_Begin(&object); \
        for(int i=0; i<position; ++i) \
        { \
            rtlibType##_Iterator_Increment(&it); \
        } \
        rtlibType##_##addMethod(&object, &it, j); \
    } \
    for(int j=0; j<oneIterationSize; ++j) \
    { \
        auto it = rtlibType##_Begin(&object); \
        for(int i=0; i<position; ++i) \
        { \
            rtlibType##_Iterator_Increment(&it); \
        } \
        rtlibType##_##deleteMethod(&object, &it); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
return duration.count();


/* Adding to stl container without iterators */
#define stlTest(stlType, addMethod, deleteMethod, oneIterationSize, iterations) \
stlType stlObject; \
\
auto start = std::chrono::high_resolution_clock::now(); \
\
cppCall(stlObject, addMethod, deleteMethod, oneIterationSize, iterations); \
\
auto stop = std::chrono::high_resolution_clock::now(); \
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
return duration.count();

/* Adding to stl container with iterators */
#define stlWithIteratorTest(stlType, addMethod, deleteMethod, oneIterationSize, iterations, position) \
stlType object{}; \
for(int i=0; i<position; ++i) \
{ \
    auto begin_it = std ::begin(object); \
    object.insert(begin_it, 0); \
} \
auto start = std::chrono::high_resolution_clock::now(); \
\
for(int i=0; i<10000000; ++i) \
{ \
    for(int j=0; j<16; ++j) \
    { \
        auto begin_it = std::begin(object); \
        for(int i=0; i<position; ++i) \
        { \
            begin_it++; \
        } \
        object.insert(begin_it, j); \
    } \
    for(int j=0; j<16; ++j) \
    { \
        auto begin_it = std::begin(object); \
        for(int i=0; i<position; ++i) \
        { \
            begin_it++; \
        } \
        object.erase(begin_it); \
    } \
} \
auto stop = std::chrono::high_resolution_clock::now(); \
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); \
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
    rtlibTest(DynamicAllocatorList, DynamicAllocatorList_PushBack, DynamicAllocatorList_PopBack, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticVectorBack()
{
    rtlibTest(TestVector, TestVector_PushBack, TestVector_PopBack, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibDynamicAllocatorVectorBack()
{
    rtlibTest(DynamicAllocatorVector, DynamicAllocatorVector_PushBack, DynamicAllocatorVector_PopBack, onIterationSize, iterations);
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
    rtlibTest(DynamicAllocatorList, DynamicAllocatorList_PushFront, DynamicAllocatorList_PopFront, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibStaticVectorFront()
{
    rtlibTest(TestVector, TestVector_PushFront, TestVector_PopFront, onIterationSize, iterations);
}

template<int onIterationSize, int iterations>
unsigned int calculateRtlibDynamicAllocatorVectorFront()
{
    rtlibTest(DynamicAllocatorVector, DynamicAllocatorVector_PushFront, DynamicAllocatorVector_PopFront, onIterationSize, iterations);
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

unsigned int calculateRtlibGenericStaticListBack_ForTest()
{
    uint32_t buf1[10000];
    List * vector = List_Init(buf1, sizeof(buf1), sizeof(int));

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            List_PushBack(vector, &j);
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
            Vector_PushBack(vector, &j);
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
    int samples[16]{};

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
    int samples[16]{};

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


void addRecordToTree(boost::property_tree::ptree& array, std::string container, unsigned int duration)
{
    boost::property_tree::ptree child;

    child.put("container", container);
    child.put("duration", duration);

    array.push_back(std::make_pair("", child));

    std::cout << container << ": " << duration << std::endl;
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
    addRecordToTree(frontTree, "rtlib-svector",calculateRtlibStaticVectorFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-cvector",calculateRtlibDynamicAllocatorVectorFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-slist",calculateRtlibStaticListFront<16, 10000000>());
    addRecordToTree(frontTree, "rtlib-clist",calculateRtlibDynamicAllocatorListFront<16, 10000000>());
    // STL vector has no push_front method
    addRecordToTree(frontTree, "stl-list",calculateSTLListFront<16, 10000000>());
    generateFile(frontTree, "front.json");

    /* Middle */
    boost::property_tree::ptree middleTree{};
    std::cout << "Middle: " << std::endl;
    addRecordToTree(middleTree, "rtlib-svector",calculateRtlibStaticVectorMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "rtlib-cvector",calculateRtlibCustomVectorMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "rtlib-slist" ,calculateRtlibStaticListMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "rtlib-clist" ,calculateRtlibCustomListMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "stl-vector", calculateSTLVectorMiddle<16, 10000000, 1>());
    addRecordToTree(middleTree, "stl-list", calculateSTLListMiddle<16, 10000000, 1>());
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