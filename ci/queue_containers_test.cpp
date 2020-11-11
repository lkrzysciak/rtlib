#include "gtest/gtest.h"
#include "containers/static_vector.h"
#include "containers/static_list.h"
#include "containers/custom_allocator_vector.h"
#include "containers/custom_allocator_list.h"
#include "memory/dynamic_allocator.h"
#include <cstdint>
#include <tuple>

#define CONTAINER_CAPACITY  5

extern "C" {
declare_static_vector_t(VectorTestType, int, CONTAINER_CAPACITY);
define_static_vector_t(VectorTestType, int, CONTAINER_CAPACITY);
declare_static_list_t(ListTestType, int, CONTAINER_CAPACITY);
define_static_list_t(ListTestType, int, CONTAINER_CAPACITY);

declare_dynamic_allocator_t(DynamicAllocator);
define_dynamic_allocator_t(DynamicAllocator);
declare_custom_allocator_vector_t(CustomAllocatorVector, int, DynamicAllocator);
define_custom_allocator_vector_t(CustomAllocatorVector, int, DynamicAllocator);
declare_custom_allocator_list_t(CustomAllocatorList, int, DynamicAllocator);
define_custom_allocator_list_t(CustomAllocatorList, int, DynamicAllocator);
}

#define create_wrappers_for_type(Type) \
void Init(Type* const container) \
{ \
    Type##_Construct(container); \
} \
\
void Deinit(Type* const container) \
{ \
    Type##_Destroy(container); \
} \
\
size_t Size(Type* const container) \
{ \
    return Type##_Size(container); \
} \
\
bool Empty(Type* const container) \
{ \
    return Type##_Empty(container); \
} \
\
int PushBack(Type* const container, int value) \
{ \
    return Type##_PushBack(container, value); \
} \
\
int PushFront(Type* const container, int value) \
{ \
    return Type##_PushFront(container, value); \
} \
\
int Insert(Type* const container, int value, Type##_iterator* it) \
{ \
    return Type##_Insert(container, it, value); \
} \
int PopBack(Type* const container) \
{ \
    return Type##_PopBack(container); \
} \
\
int PopFront(Type* const container) \
{ \
    return Type##_PopFront(container); \
} \
\
int Erase(Type* const container, Type##_iterator* it) \
{ \
    return Type##_Erase(container, it); \
} \
\
int Back(Type* const container) \
{ \
    return Type##_Back(container); \
} \
\
int Front(Type* const container) \
{ \
    return Type##_Front(container); \
} \
\
auto Begin(Type* const container) \
{ \
    return Type##_Begin(container); \
} \
\
auto End(Type* const container) \
{ \
    return Type##_End(container); \
} \
\
auto GetValue(Type* const container, size_t index) \
{ \
    return Type##_GetValue(container, index); \
} \
\
void SetValue(Type* const container, size_t index, int value) \
{ \
    Type##_SetValue(container, index, value); \
} \
\
auto IteratorValue(Type##_iterator* const it) \
{ \
    return Type##_Iterator_GetValue(it); \
} \
\
void IteratorInc(Type##_iterator* const it) \
{ \
    return Type##_Iterator_Increment(it); \
} \
\
void IteratorDec(Type##_iterator* const it) \
{ \
    return Type##_Iterator_Decrement(it); \
} \
\
void IteratorSetValue(Type##_iterator* const it, int value) \
{ \
    Type##_Iterator_SetValue(it, value); \
} \
\
bool Iterator_Equal(Type##_iterator* const first, Type##_iterator* const second) \
{ \
    return Type##_Iterator_Equal(first, second); \
} \
\
auto Find(Type* const container, int value, bool(*fun)(const int*, const int*)) \
{ \
    return Type##_Find(container, value, fun); \
}


create_wrappers_for_type(VectorTestType);
create_wrappers_for_type(ListTestType);
create_wrappers_for_type(CustomAllocatorVector);
create_wrappers_for_type(CustomAllocatorList);


template<typename T>
struct ContainerTest : public testing::Test
{
    void SetUp() override 
    {
        Init(&container);
    }

    void TearDown() override 
    {
        Deinit(&container);
    }

    T container;
};

template<typename T>
struct StaticContainerTest : public testing::Test
{
    void SetUp() override 
    {
        Init(&container);
    }

    void TearDown() override 
    {
        Deinit(&container);
    }

    T container;
};

using MyTypes = testing::Types<
    VectorTestType,
    ListTestType,
    CustomAllocatorVector,
    CustomAllocatorList
        >;

using StaticContainerTypes = testing::Types<
    VectorTestType,
    ListTestType
        >;

TYPED_TEST_SUITE(ContainerTest, MyTypes);
TYPED_TEST_SUITE(StaticContainerTest, StaticContainerTypes);

TYPED_TEST(ContainerTest, IsEmptyAfterInit)
{
    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PushBack)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(PushBack(&this->container, temp3), 3);
    auto it = Begin(&this->container);
    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp3);
    
    ASSERT_EQ(Size(&this->container), 3);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PushFront)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    ASSERT_EQ(PushFront(&this->container, temp1), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(PushFront(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp2);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(PushFront(&this->container, temp3), 3);
    auto it = Begin(&this->container);
    ASSERT_EQ(IteratorValue(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp1);

    ASSERT_EQ(Size(&this->container), 3);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, Insert)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto it = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, temp1, &it), 1);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp2, &it), 2);
    
    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp3, &it), 3);
    
    it = Begin(&this->container);
    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);

    ASSERT_EQ(Size(&this->container), 3);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, Erase)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto it = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, temp1, &it), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp2, &it), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp3, &it), 3);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);
    
    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&this->container, &it), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);
    
    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&this->container, &it), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    it = Begin(&this->container);
    ASSERT_EQ(Erase(&this->container, &it), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PopBack)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(PopBack(&this->container), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);
    
    ASSERT_EQ(PopBack(&this->container), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PopFront)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(PopFront(&this->container), 1);
    ASSERT_EQ(Front(&this->container), temp2);
    ASSERT_EQ(Back(&this->container), temp2);
    
    ASSERT_EQ(PopFront(&this->container), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, BeginEndIterator)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it = End(&this->container);

    IteratorDec(&it);

    ASSERT_EQ(IteratorValue(&it_begin), temp1);
    ASSERT_EQ(IteratorValue(&it), temp2);
}

TYPED_TEST(ContainerTest, IteratorIncrementation)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it_end = End(&this->container);
    auto it = it_begin;

    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_TRUE(Iterator_Equal(&it, &it_end));
}

TYPED_TEST(ContainerTest, IteratorDecrementation)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it_end = End(&this->container);

    auto it = it_end;
    IteratorDec(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorDec(&it);
    ASSERT_EQ(IteratorValue(&it), temp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}

TYPED_TEST(ContainerTest, IteratorAfterInit)
{
    auto it_begin = Begin(&this->container);
    auto it_end = End(&this->container);

    ASSERT_TRUE(Iterator_Equal(&it_begin, &it_end));
}

TYPED_TEST(ContainerTest, IteratorValues)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it_end = End(&this->container);

    auto it = it_end;
    IteratorDec(&it);
    uint32_t newTemp2{ 2468 };
    IteratorSetValue(&it, newTemp2);
    ASSERT_EQ(IteratorValue(&it), newTemp2);
    IteratorDec(&it);
    uint32_t newTemp1{ 1357 };
    IteratorSetValue(&it, newTemp1);
    ASSERT_EQ(IteratorValue(&it), newTemp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}

TYPED_TEST(ContainerTest, IndexValues)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(GetValue(&this->container, 0), temp1);
    ASSERT_EQ(GetValue(&this->container, 1), temp2);

    uint32_t newTemp1{ 1357 };
    uint32_t newTemp2{ 2468 };

    SetValue(&this->container, 0, newTemp1);
    ASSERT_EQ(GetValue(&this->container, 0), newTemp1);
    ASSERT_EQ(GetValue(&this->container, 1), temp2);

    SetValue(&this->container, 1, newTemp2);
    ASSERT_EQ(GetValue(&this->container, 0), newTemp1);
    ASSERT_EQ(GetValue(&this->container, 1), newTemp2);
}

bool compare_ints(const int* v1, const int* v2)
{
    return *v1 == *v2;
}

TYPED_TEST(ContainerTest, FindAllValues)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 582 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);

    auto end_it = End(&this->container);

    auto it_1 = Find(&this->container, temp1, compare_ints);
    ASSERT_EQ(IteratorValue(&it_1), temp1);
    auto it_2 = Find(&this->container, temp2, compare_ints);
    ASSERT_EQ(IteratorValue(&it_2), temp2);
    auto it_3 = Find(&this->container, temp3, compare_ints);
    ASSERT_EQ(IteratorValue(&it_3), temp3);

    auto it_4 = Find(&this->container, temp3 + 1, compare_ints);
    ASSERT_TRUE(Iterator_Equal(&it_4, &end_it));
}

TYPED_TEST(StaticContainerTest, PushBackOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i=0; i<CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushBack(&this->container, temp1), i + 1);
    }
    ASSERT_EQ(PushBack(&this->container, temp1), ALLOCATION_ERROR);
}

TYPED_TEST(StaticContainerTest, PushFrontOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i=0; i<CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushFront(&this->container, temp1), i + 1);
    }
    ASSERT_EQ(PushFront(&this->container, temp1), ALLOCATION_ERROR);
}

TYPED_TEST(StaticContainerTest, InsertOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i=0; i<CONTAINER_CAPACITY; ++i)
    {
        auto it = Begin(&this->container);
        ASSERT_EQ(Insert(&this->container, temp1, &it), i + 1);
    }
    auto it = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, temp1, &it), ALLOCATION_ERROR);
}