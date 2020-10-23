#include "gtest/gtest.h"
#include "for_tests/vector.h"
#include <cstdint>

TEST(VectorTest, Init)
{
    uint32_t buf[100];
    Vector * vector = Vector_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(Vector_Size(vector), 0);
}

TEST(VectorTest, PushBack)
{
    uint32_t buf[100];
    Vector * vector = Vector_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(Vector_Size(vector), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(Vector_PushBack(vector, &temp1), 1);
    ASSERT_EQ(*(uint32_t *)Vector_Front(vector), temp1);
    ASSERT_EQ(*(uint32_t *)Vector_Back(vector), temp1);

    ASSERT_EQ(Vector_PushBack(vector, &temp2), 2);
    ASSERT_EQ(*(uint32_t *)Vector_Front(vector), temp1);
    ASSERT_EQ(*(uint32_t *)Vector_Back(vector), temp2);
}

TEST(VectorTest, PushFront)
{
    uint32_t buf[100];
    Vector * vector = Vector_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(Vector_Size(vector), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(Vector_PushFront(vector, &temp1), 1);
    ASSERT_EQ(*(uint32_t *)Vector_Front(vector), temp1);
    ASSERT_EQ(*(uint32_t *)Vector_Back(vector), temp1);

    ASSERT_EQ(Vector_PushFront(vector, &temp2), 2);
    ASSERT_EQ(*(uint32_t *)Vector_Front(vector), temp2);
    ASSERT_EQ(*(uint32_t *)Vector_Back(vector), temp1);
}

TEST(VectorTest, MixedPushFrontBack)
{
    uint32_t buf[100];
    Vector * vector = Vector_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(Vector_Size(vector), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };

    ASSERT_EQ(Vector_PushFront(vector, &temp1), 1);
    ASSERT_EQ(*(uint32_t *)Vector_Front(vector), temp1);
    ASSERT_EQ(*(uint32_t *)Vector_Back(vector), temp1);

    ASSERT_EQ(Vector_PushFront(vector, &temp2), 2);
    ASSERT_EQ(*(uint32_t *)Vector_Front(vector), temp2);
    ASSERT_EQ(*(uint32_t *)Vector_Back(vector), temp1);

    ASSERT_EQ(Vector_PushBack(vector, &temp3), 3);
    ASSERT_EQ(*(uint32_t *)Vector_Front(vector), temp2);
    ASSERT_EQ(*(uint32_t *)Vector_Back(vector), temp3);
}