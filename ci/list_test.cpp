#include "gtest/gtest.h"
#include "containers/list.h"
#include <cstdint>

TEST(ListTest, Init)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);
}

TEST(ListTest, PushBack)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(List_PushBack(list, &temp1), 1);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp1);

    ASSERT_EQ(List_PushBack(list, &temp2), 2);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp2);
}

TEST(ListTest, PushFront)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(List_PushFront(list, &temp1), 1);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp1);

    ASSERT_EQ(List_PushFront(list, &temp2), 2);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp1);
}

TEST(ListTest, MixedPushFrontBack)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };

    ASSERT_EQ(List_PushFront(list, &temp1), 1);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp1);

    ASSERT_EQ(List_PushFront(list, &temp2), 2);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp1);

    ASSERT_EQ(List_PushBack(list, &temp3), 3);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp3);
}

TEST(ListTest, PushFrontIterator)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };

    ASSERT_EQ(List_PushFront(list, &temp1), 1);
    ASSERT_EQ(List_PushFront(list, &temp2), 2);
    ASSERT_EQ(List_PushFront(list, &temp3), 3);

    ListIterator* it    = List_Begin(list);
    ListIterator* endIt = List_End(list);

    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp3);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp2);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp1);
    it = ListIterator_Increment(it);
    ListIterator_Equal(it, endIt);
}

TEST(ListTest, PushBackIterator)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };

    ASSERT_EQ(List_PushBack(list, &temp1), 1);
    ASSERT_EQ(List_PushBack(list, &temp2), 2);
    ASSERT_EQ(List_PushBack(list, &temp3), 3);

    ListIterator* it    = List_Begin(list);
    ListIterator* endIt = List_End(list);

    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp1);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp2);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp3);
    it = ListIterator_Increment(it);
    ListIterator_Equal(it, endIt);
}

TEST(ListTest, PopBack)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };

    ASSERT_EQ(List_PushBack(list, &temp1), 1);
    ASSERT_EQ(List_PushBack(list, &temp2), 2);
    ASSERT_EQ(List_PushBack(list, &temp3), 3);

    ASSERT_EQ(*(uint32_t *)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp3);

    ASSERT_EQ(List_PopBack(list), 2);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp2);

    ASSERT_EQ(List_PopBack(list), 1);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp1);

    ASSERT_EQ(List_PopBack(list), 0);

    ASSERT_EQ(List_PushBack(list, &temp2), 1);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp2);
}

TEST(ListTest, PopFront)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };

    List_PushBack(list, &temp1);
    List_PushBack(list, &temp2);
    List_PushBack(list, &temp3);

    ASSERT_EQ(List_PopFront(list), 2);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp3);

    ASSERT_EQ(List_PopFront(list), 1);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp3);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp3);

    ASSERT_EQ(List_PopFront(list), 0);

    ASSERT_EQ(List_PushBack(list, &temp2), 1);
    ASSERT_EQ(*(uint32_t *)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t *)List_Back(list), temp2);
}

TEST(ListTest, Insert)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };
    uint32_t temp4{ 8999 };
    uint32_t temp5{ 12345 };

    ListIterator* it = List_Begin(list);
    ASSERT_EQ(List_Insert(list, &temp1, it), 1);

    it = List_Begin(list);  // update begin iterator
    ASSERT_EQ(List_Insert(list, &temp2, it), 2);

    it = List_Begin(list);  // update begin iterator
    it = ListIterator_Increment(it);

    ASSERT_EQ(List_Insert(list, &temp3, it), 3);

    it = List_Begin(list);  // update begin iterator
    it = ListIterator_Increment(it);
    ASSERT_EQ(List_Insert(list, &temp4, it), 4);

    it = List_Begin(list);  // update begin iterator
    it = ListIterator_Increment(it);
    ASSERT_EQ(List_Insert(list, &temp5, it), 5);

    ListIterator* toCompareIt = List_Begin(list);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(toCompareIt), temp2);
    toCompareIt = ListIterator_Increment(toCompareIt);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(toCompareIt), temp5);
    toCompareIt = ListIterator_Increment(toCompareIt);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(toCompareIt), temp4);
    toCompareIt = ListIterator_Increment(toCompareIt);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(toCompareIt), temp3);
    toCompareIt = ListIterator_Increment(toCompareIt);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(toCompareIt), temp1);
    toCompareIt = ListIterator_Increment(toCompareIt);
    ListIterator* end = List_End(list);
    ASSERT_TRUE(ListIterator_Equal(toCompareIt, end));
}

TEST(ListTest, Erase)
{
    uint32_t buf[100];
    List * list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 555 };
    uint32_t temp4{ 8999 };
    uint32_t temp5{ 12345 };

    List_PushBack(list, &temp1);
    List_PushBack(list, &temp2);
    List_PushBack(list, &temp3);
    List_PushBack(list, &temp4);
    List_PushBack(list, &temp5);

    ListIterator* it = List_Begin(list);

    ASSERT_EQ(List_Erase(list, it), 4);
    it = List_Begin(list);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp2);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp3);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp4);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp5);

    ASSERT_EQ(List_Erase(list, it), 3);
    it = List_Begin(list);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp2);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp3);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp4);
    it = ListIterator_Decrement(it);

    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp3);

    ASSERT_EQ(List_Erase(list, it), 2);
    it = List_Begin(list);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp2);
    it = ListIterator_Increment(it);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp4);

    ASSERT_EQ(List_Erase(list, it), 1);
    it = List_Begin(list);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp2);

    ASSERT_EQ(List_Erase(list, it), 0);

    ASSERT_EQ(List_PushBack(list, &temp1), 1);
    it = List_Begin(list);
    ASSERT_EQ(*(uint32_t *)ListIterator_Value(it), temp1);
}
