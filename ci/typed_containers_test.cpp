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

template<typename T>
struct ContainerTest : public testing::Test
{};

using MyTypes = testing::Types<
    VectorTestType,
    ListTestType
        >;
TYPED_TEST_CASE(ContainerTest, MyTypes);

TYPED_TEST(ContainerTest, MyTestName)
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
