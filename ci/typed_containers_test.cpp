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

int PushFront(VectorTestType* const vector, int value)
{
    return VectorTestType_PushFront(vector, value);
}

int PushFront(ListTestType* const list, int value)
{
    return ListTestType_PushFront(list, value);
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

int PopBack(VectorTestType* const vector)
{
    return VectorTestType_PopBack(vector);
}

int PopBack(ListTestType* const list)
{
    return ListTestType_PopBack(list);
}

int PopFront(VectorTestType* const vector)
{
    return VectorTestType_PopFront(vector);
}

int PopFront(ListTestType* const list)
{
    return ListTestType_PopFront(list);
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
    return VectorTestType_Iterator_Next(it);
}

auto IteratorInc(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_Next(it);
}

auto IteratorDec(VectorTestType_iterator* const it)
{
    return VectorTestType_Iterator_Prev(it);
}

auto IteratorDec(ListTestType_iterator* const it)
{
    return ListTestType_Iterator_Prev(it);
}

void IteratorSetValue(VectorTestType_iterator* const it, int value)
{
    VectorTestType_Iterator_SetValue(it, value);
}

void IteratorSetValue(ListTestType_iterator* const it, int value)
{
    ListTestType_Iterator_SetValue(it, value);
}

bool Iterator_Equal(VectorTestType_iterator* const first, VectorTestType_iterator* const second)
{
    return VectorTestType_Iterator_Equal(first, second);
}

bool Iterator_Equal(ListTestType_iterator* const first, ListTestType_iterator* const second)
{
    return ListTestType_Iterator_Equal(first, second);
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

TYPED_TEST(ContainerTest, PushFront)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushFront(&container, temp1), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    ASSERT_EQ(PushFront(&container, temp2), 2);
    ASSERT_EQ(Front(&container), temp2);
    ASSERT_EQ(Back(&container), temp1);
}

TYPED_TEST(ContainerTest, Insert)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto begin_it = Begin(&container);
    ASSERT_EQ(Insert(&container, temp1, &begin_it), 1);

    begin_it = Begin(&container);
    auto it_1 = IteratorInc(&begin_it);
    ASSERT_EQ(Insert(&container, temp2, &it_1), 2);
    
    begin_it = Begin(&container);
    auto it_2 = IteratorInc(&begin_it);
    ASSERT_EQ(Insert(&container, temp3, &it_2), 3);
    
    auto it = Begin(&container);
    ASSERT_EQ(IteratorValue(&it), temp1);
    it = IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp3);
    it = IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
}

TYPED_TEST(ContainerTest, Erase)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto begin_it = Begin(&container);
    ASSERT_EQ(Insert(&container, temp1, &begin_it), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    begin_it = Begin(&container);
    auto it_1 = IteratorInc(&begin_it);
    ASSERT_EQ(Insert(&container, temp2, &it_1), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);
    
    begin_it = Begin(&container);
    auto it_2 = IteratorInc(&begin_it);
    ASSERT_EQ(Insert(&container, temp3, &it_2), 3);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);

    begin_it = Begin(&container);
    auto _2nd_iterator = IteratorInc(&begin_it);
    ASSERT_EQ(Erase(&container, &_2nd_iterator), 2);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp2);
    
    begin_it = Begin(&container);
    _2nd_iterator = IteratorInc(&begin_it);
    ASSERT_EQ(Erase(&container, &_2nd_iterator), 1);
    ASSERT_EQ(Front(&container), temp1);
    ASSERT_EQ(Back(&container), temp1);

    begin_it = Begin(&container);
    ASSERT_EQ(Erase(&container, &begin_it), 0);
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

    auto it = IteratorDec(&it_end);

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
    it = IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    it = IteratorInc(&it);
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
    it = IteratorDec(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    it = IteratorDec(&it);
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
    it = IteratorDec(&it);
    uint32_t newTemp2{ 2468 };
    IteratorSetValue(&it, newTemp2);
    ASSERT_EQ(IteratorValue(&it), newTemp2);
    it = IteratorDec(&it);
    uint32_t newTemp1{ 1357 };
    IteratorSetValue(&it, newTemp1);
    ASSERT_EQ(IteratorValue(&it), newTemp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}