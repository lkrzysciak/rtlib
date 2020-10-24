#include "gtest/gtest.h"
#include "containers/static_vector.h"
#include "containers/static_list.h"
#include "containers/custom_allocator_vector.h"
#include "memory/dynamic_allocator.h"
#include <cstdint>
#include <tuple>

#define CONTAINER_CAPACITY  5

declare_static_vector_t(VectorTestType, int, CONTAINER_CAPACITY);
define_static_vector_t(VectorTestType, int, CONTAINER_CAPACITY);
declare_static_list_t(ListTestType, int, CONTAINER_CAPACITY);
define_static_list_t(ListTestType, int, CONTAINER_CAPACITY);

declare_dynamic_allocator_t(DynamicAllocator, int);
define_dynamic_allocator_t(DynamicAllocator, int);
declare_custom_allocator_vector_t(CustomAllocatorVector, int, DynamicAllocator);
define_custom_allocator_vector_t(CustomAllocatorVector, int, DynamicAllocator);

void Init(VectorTestType* const vector)
{
    VectorTestType_Init(vector);
}

void Init(ListTestType* const list)
{
    ListTestType_Init(list);
}

void Init(CustomAllocatorVector* const list)
{
    CustomAllocatorVector_Construct(list);
}

size_t Size(VectorTestType* const vector)
{
    return VectorTestType_Size(vector);
}

size_t Size(ListTestType* const list)
{
    return ListTestType_Size(list);
}

size_t Size(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_Size(list);
}

bool Empty(VectorTestType* const vector)
{
    return VectorTestType_Empty(vector);
}

bool Empty(ListTestType* const list)
{
    return ListTestType_Empty(list);
}

bool Empty(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_Empty(list);
}

int PushBack(VectorTestType* const vector, int value)
{
    return VectorTestType_PushBack(vector, value);
}

int PushBack(ListTestType* const list, int value)
{
    return ListTestType_PushBack(list, value);
}

int PushBack(CustomAllocatorVector* const list, int value)
{
    return CustomAllocatorVector_PushBack(list, value);
}

int PushFront(VectorTestType* const vector, int value)
{
    return VectorTestType_PushFront(vector, value);
}

int PushFront(ListTestType* const list, int value)
{
    return ListTestType_PushFront(list, value);
}

int PushFront(CustomAllocatorVector* const list, int value)
{
    return CustomAllocatorVector_PushFront(list, value);
}

template<typename iterator_t>
int Insert(VectorTestType* const vector, int value, iterator_t* it)
{
    return VectorTestType_Insert(vector, it, value);
}

template<typename iterator_t>
int Insert(ListTestType* const list, int value, iterator_t* it)
{
    return ListTestType_Insert(list, it, value);
}

template<typename iterator_t>
int Insert(CustomAllocatorVector* const list, int value, iterator_t* it)
{
    return CustomAllocatorVector_Insert(list, it, value);
}

int PopBack(VectorTestType* const vector)
{
    return VectorTestType_PopBack(vector);
}

int PopBack(ListTestType* const list)
{
    return ListTestType_PopBack(list);
}

int PopBack(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_PopBack(list);
}

int PopFront(VectorTestType* const vector)
{
    return VectorTestType_PopFront(vector);
}

int PopFront(ListTestType* const list)
{
    return ListTestType_PopFront(list);
}

int PopFront(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_PopFront(list);
}

template<typename iterator_t>
int Erase(VectorTestType* const vector, iterator_t* it)
{
    return VectorTestType_Erase(vector, it);
}

template<typename iterator_t>
int Erase(ListTestType* const list, iterator_t* it)
{
    return ListTestType_Erase(list, it);
}

template<typename iterator_t>
int Erase(CustomAllocatorVector* const list, iterator_t* it)
{
    return CustomAllocatorVector_Erase(list, it);
}

int Back(VectorTestType* const vector)
{
    return VectorTestType_Back(vector);
}

int Back(ListTestType* const list)
{
    return ListTestType_Back(list);
}

int Back(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_Back(list);
}

int Front(VectorTestType* const vector)
{
    return VectorTestType_Front(vector);
}

int Front(ListTestType* const list)
{
    return ListTestType_Front(list);
}

int Front(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_Front(list);
}

auto Begin(VectorTestType* const vector)
{
    return VectorTestType_Begin(vector);
}

auto Begin(ListTestType* const list)
{
    return ListTestType_Begin(list);
}

auto Begin(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_Begin(list);
}

auto End(VectorTestType* const vector)
{
    return VectorTestType_End(vector);
}

auto End(ListTestType* const list)
{
    return ListTestType_End(list);
}

auto End(CustomAllocatorVector* const list)
{
    return CustomAllocatorVector_End(list);
}

auto GetValue(VectorTestType* const vector, size_t index)
{
    return VectorTestType_GetValue(vector, index);
}

auto GetValue(ListTestType* const list, size_t index)
{
    return ListTestType_GetValue(list, index);
}

auto GetValue(CustomAllocatorVector* const list, size_t index)
{
    return CustomAllocatorVector_GetValue(list, index);
}

void SetValue(VectorTestType* const vector, size_t index, int value)
{
    VectorTestType_SetValue(vector, index, value);
}

void SetValue(ListTestType* const list, size_t index, int value)
{
    ListTestType_SetValue(list, index, value);
}

void SetValue(CustomAllocatorVector* const list, size_t index, int value)
{
    CustomAllocatorVector_SetValue(list, index, value);
}

auto IteratorValue(VectorTestType_iterator* const it)
{
    return VectorTestType_Iterator_GetValue(it);
}

auto IteratorValue(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_GetValue(it);
}

auto IteratorValue(CustomAllocatorVector_iterator* const it)
{
    return CustomAllocatorVector_Iterator_GetValue(it);
}

void IteratorInc(VectorTestType_iterator* const it)
{
    return VectorTestType_Iterator_Increment(it);
}

void IteratorInc(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_Increment(it);
}

void IteratorInc(CustomAllocatorVector_iterator* const it)
{
    return CustomAllocatorVector_Iterator_Increment(it);
}

void IteratorDec(VectorTestType_iterator* const it)
{
    return VectorTestType_Iterator_Decrement(it);
}

void IteratorDec(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_Decrement(it);
}

void IteratorDec(CustomAllocatorVector_iterator* const it)
{
    return CustomAllocatorVector_Iterator_Decrement(it);
}

void IteratorSetValue(VectorTestType_iterator* const it, int value)
{
    VectorTestType_Iterator_SetValue(it, value);
}

void IteratorSetValue(ListTestType_iterator* const it, int value)
{
    ListTestType_Iterator_SetValue(it, value);
}

void IteratorSetValue(CustomAllocatorVector_iterator* const it, int value)
{
    CustomAllocatorVector_Iterator_SetValue(it, value);
}

bool Iterator_Equal(VectorTestType_iterator* const first, VectorTestType_iterator* const second)
{
    return VectorTestType_Iterator_Equal(first, second);
}

bool Iterator_Equal(ListTestType_iterator* const first, ListTestType_iterator* const second)
{
    return ListTestType_Iterator_Equal(first, second);
}

bool Iterator_Equal(CustomAllocatorVector_iterator* const first, CustomAllocatorVector_iterator* const second)
{
    return CustomAllocatorVector_Iterator_Equal(first, second);
}

auto Find(VectorTestType* const vector, int value, bool(*fun)(const int*, const int*))
{
    return VectorTestType_Find(vector, value, fun);
}

auto Find(ListTestType* const list, int value, bool(*fun)(const int*, const int*))
{
    return ListTestType_Find(list, value, fun);
}

auto Find(CustomAllocatorVector* const list, int value, bool(*fun)(const int*, const int*))
{
    return CustomAllocatorVector_Find(list, value, fun);
}

template<typename T>
struct ContainerTest : public testing::Test
{};

template<typename T>
struct StaticContainerTest : public testing::Test
{};

using MyTypes = testing::Types<
    VectorTestType,
    ListTestType
        >;

using StaticContainerTypes = testing::Types<
    VectorTestType,
    ListTestType
        >;

TYPED_TEST_CASE(ContainerTest, MyTypes);
TYPED_TEST_CASE(StaticContainerTest, StaticContainerTypes);

TYPED_TEST(ContainerTest, Init)
{
    TypeParam container{};
    Init(&container);
}

TYPED_TEST(ContainerTest, IsEmptyAfterInit)
{
    TypeParam container{};
    Init(&container);

    ASSERT_EQ(Size(&container), 0);
    ASSERT_TRUE(Empty(&container));
}

TYPED_TEST(ContainerTest, PushBack)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    ASSERT_EQ(PushBack(&container, temp1), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    ASSERT_EQ(PushBack(&container, temp2), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);

    ASSERT_EQ(PushBack(&container, temp3), 3);
    auto it = Begin(&container);
    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp3);
    
    ASSERT_EQ(Size(&container), 3);
    ASSERT_FALSE(Empty(&container));
}

TYPED_TEST(ContainerTest, PushFront)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    ASSERT_EQ(PushFront(&container, temp1), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    ASSERT_EQ(PushFront(&container, temp2), 2);
    ASSERT_EQ(Front(&container), temp2);
    ASSERT_EQ(Back(&container), temp1);

    ASSERT_EQ(PushFront(&container, temp3), 3);
    auto it = Begin(&container);
    ASSERT_EQ(IteratorValue(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp1);

    ASSERT_EQ(Size(&container), 3);
    ASSERT_FALSE(Empty(&container));
}

TYPED_TEST(ContainerTest, Insert)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto it = Begin(&container);
    ASSERT_EQ(Insert(&container, temp1, &it), 1);

    it = Begin(&container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&container, temp2, &it), 2);
    
    it = Begin(&container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&container, temp3, &it), 3);
    
    it = Begin(&container);
    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);

    ASSERT_EQ(Size(&container), 3);
    ASSERT_FALSE(Empty(&container));
}

TYPED_TEST(ContainerTest, Erase)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto it = Begin(&container);
    ASSERT_EQ(Insert(&container, temp1, &it), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    it = Begin(&container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&container, temp2, &it), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);

    it = Begin(&container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&container, temp3, &it), 3);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);
    
    it = Begin(&container);
    IteratorInc(&it);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);

    it = Begin(&container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&container, &it), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);
    
    it = Begin(&container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&container, &it), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    it = Begin(&container);
    ASSERT_EQ(Erase(&container, &it), 0);

    ASSERT_EQ(Size(&container), 0);
    ASSERT_TRUE(Empty(&container));
}

TYPED_TEST(ContainerTest, PopBack)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushBack(&container, temp1), 1);
    ASSERT_EQ(PushBack(&container, temp2), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);

    ASSERT_EQ(PopBack(&container), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);
    
    ASSERT_EQ(PopBack(&container), 0);

    ASSERT_EQ(Size(&container), 0);
    ASSERT_TRUE(Empty(&container));
}

TYPED_TEST(ContainerTest, PopFront)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushBack(&container, temp1), 1);
    ASSERT_EQ(PushBack(&container, temp2), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);

    ASSERT_EQ(PopFront(&container), 1);
    ASSERT_EQ(Front(&container), temp2);
    ASSERT_EQ(Back(&container), temp2);
    
    ASSERT_EQ(PopFront(&container), 0);

    ASSERT_EQ(Size(&container), 0);
    ASSERT_TRUE(Empty(&container));
}

TYPED_TEST(ContainerTest, BeginEndIterator)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&container, temp1);
    PushBack(&container, temp2);

    auto it_begin = Begin(&container);
    auto it = End(&container);

    IteratorDec(&it);

    ASSERT_EQ(IteratorValue(&it_begin), temp1);
    ASSERT_EQ(IteratorValue(&it), temp2);
}

TYPED_TEST(ContainerTest, IteratorIncrementation)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&container, temp1);
    PushBack(&container, temp2);

    auto it_begin = Begin(&container);
    auto it_end = End(&container);
    auto it = it_begin;

    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_TRUE(Iterator_Equal(&it, &it_end));
}

TYPED_TEST(ContainerTest, IteratorDecrementation)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&container, temp1);
    PushBack(&container, temp2);

    auto it_begin = Begin(&container);
    auto it_end = End(&container);

    auto it = it_end;
    IteratorDec(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorDec(&it);
    ASSERT_EQ(IteratorValue(&it), temp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}

TYPED_TEST(ContainerTest, IteratorAfterInit)
{
    TypeParam container{};
    Init(&container);

    auto it_begin = Begin(&container);
    auto it_end = End(&container);

    ASSERT_TRUE(Iterator_Equal(&it_begin, &it_end));
}

TYPED_TEST(ContainerTest, IteratorValues)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&container, temp1);
    PushBack(&container, temp2);

    auto it_begin = Begin(&container);
    auto it_end = End(&container);

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
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&container, temp1);
    PushBack(&container, temp2);

    ASSERT_EQ(GetValue(&container, 0), temp1);
    ASSERT_EQ(GetValue(&container, 1), temp2);

    uint32_t newTemp1{ 1357 };
    uint32_t newTemp2{ 2468 };

    SetValue(&container, 0, newTemp1);
    ASSERT_EQ(GetValue(&container, 0), newTemp1);
    ASSERT_EQ(GetValue(&container, 1), temp2);

    SetValue(&container, 1, newTemp2);
    ASSERT_EQ(GetValue(&container, 0), newTemp1);
    ASSERT_EQ(GetValue(&container, 1), newTemp2);
}

bool compare_ints(const int* v1, const int* v2)
{
    return *v1 == *v2;
}

TYPED_TEST(ContainerTest, FindAllValues)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 582 };

    PushBack(&container, temp1);
    PushBack(&container, temp2);
    PushBack(&container, temp3);

    auto end_it = End(&container);

    auto it_1 = Find(&container, temp1, compare_ints);
    ASSERT_EQ(IteratorValue(&it_1), temp1);
    auto it_2 = Find(&container, temp2, compare_ints);
    ASSERT_EQ(IteratorValue(&it_2), temp2);
    auto it_3 = Find(&container, temp3, compare_ints);
    ASSERT_EQ(IteratorValue(&it_3), temp3);

    auto it_4 = Find(&container, temp3 + 1, compare_ints);
    ASSERT_TRUE(Iterator_Equal(&it_4, &end_it));
}

TYPED_TEST(StaticContainerTest, PushBackOverLimit)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };

    for(int i=0; i<CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushBack(&container, temp1), i + 1);
    }
    ASSERT_EQ(PushBack(&container, temp1), ALLOCATION_ERROR);
}

TYPED_TEST(StaticContainerTest, PushFrontOverLimit)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };

    for(int i=0; i<CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushFront(&container, temp1), i + 1);
    }
    ASSERT_EQ(PushFront(&container, temp1), ALLOCATION_ERROR);
}

TYPED_TEST(StaticContainerTest, InsertOverLimit)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };

    for(int i=0; i<CONTAINER_CAPACITY; ++i)
    {
        auto it = Begin(&container);
        ASSERT_EQ(Insert(&container, temp1, &it), i + 1);
    }
    auto it = Begin(&container);
    ASSERT_EQ(Insert(&container, temp1, &it), ALLOCATION_ERROR);
}