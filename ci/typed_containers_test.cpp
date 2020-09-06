#include "gtest/gtest.h"
#include "containers/typed_vector.h"
#include "containers/typed_list.h"
#include <cstdint>
#include <tuple>

typed_vector_t(VectorTestType, int, 5);
typed_list_t(ListTestType, int, 5);

void Init(VectorTestType* const vector)
{
    VectorTestType_Init(vector);
}

void Init(ListTestType* const list)
{
    ListTestType_Init(list);
}

int PushBack(VectorTestType* const vector, int value)
{
    return VectorTestType_PushBack(vector, value);
}

int PushBack(ListTestType* const list, int value)
{
    return ListTestType_PushBack(list, value);
}

int PopBack(VectorTestType* const vector)
{
    return VectorTestType_PopBack(vector);
}

int PopBack(ListTestType* const list)
{
    return ListTestType_PopBack(list);
}

int Back(VectorTestType* const vector)
{
    return VectorTestType_Back(vector);
}

int Back(ListTestType* const list)
{
    return ListTestType_Back(list);
}

int Front(VectorTestType* const vector)
{
    return VectorTestType_Front(vector);
}

int Front(ListTestType* const list)
{
    return ListTestType_Front(list);
}

auto Begin(VectorTestType* const vector)
{
    return VectorTestType_Begin(vector);
}

auto Begin(ListTestType* const list)
{
    return ListTestType_Begin(list);
}

auto End(VectorTestType* const vector)
{
    return VectorTestType_End(vector);
}

auto End(ListTestType* const list)
{
    return ListTestType_End(list);
}

auto IteratorValue(VectorTestType_iterator* const it)
{
    return VectorTestType_Iterator_GetValue(it);
}

auto IteratorValue(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_GetValue(it);
}

auto IteratorInc(VectorTestType_iterator* const it)
{
    return VectorTestType_Iterator_Increment(it);
}

auto IteratorInc(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_Increment(it);
}

auto IteratorDec(VectorTestType_iterator* const it)
{
    return VectorTestType_Iterator_Decrement(it);
}

auto IteratorDec(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_Decrement(it);
}

template<typename T>
struct ContainerTest : public testing::Test
{};

using MyTypes = testing::Types<
    VectorTestType,
    ListTestType
        >;
TYPED_TEST_CASE(ContainerTest, MyTypes);

TYPED_TEST(ContainerTest, Init)
{
    TypeParam container{};
    Init(&container);
}

TYPED_TEST(ContainerTest, PushBack)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushBack(&container, temp1), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    ASSERT_EQ(PushBack(&container, temp2), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);
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
    auto it_end = End(&container);

    auto it = IteratorDec(it_end);

    ASSERT_EQ(IteratorValue(it_begin), temp1);
    ASSERT_EQ(IteratorValue(it), temp2);
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

    ASSERT_EQ(IteratorValue(it), temp1);
    it = IteratorInc(it);
    ASSERT_EQ(IteratorValue(it), temp2);
    it = IteratorInc(it);
    ASSERT_EQ(it, it_end);
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
    it = IteratorDec(it);
    ASSERT_EQ(IteratorValue(it), temp2);
    it = IteratorDec(it);
    ASSERT_EQ(IteratorValue(it), temp1);
    ASSERT_EQ(it, it_begin);
}

TYPED_TEST(ContainerTest, IteratorAfterInit)
{
    TypeParam container{};
    Init(&container);

    auto it_begin = Begin(&container);
    auto it_end = End(&container);

    ASSERT_EQ(it_begin, it_end);
}