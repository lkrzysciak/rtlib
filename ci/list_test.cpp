#include "gtest/gtest.h"
#include "containers/list.h"
#include "containers/list.c"
#include <cstdint>


TEST(ListTest, Init)
{
    uint32_t buf[100];
    List* list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);
}

TEST(ListTest, PushBack)
{
    uint32_t buf[100];
    List* list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{3215};
    uint32_t temp2{23587};

    ASSERT_EQ(List_PushBack(list, &temp1), 1);
    ASSERT_EQ(*(uint32_t*)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t*)List_Back(list), temp1);

    ASSERT_EQ(List_PushBack(list, &temp2), 2);
    ASSERT_EQ(*(uint32_t*)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t*)List_Back(list), temp2);
}

TEST(ListTest, PushFront)
{
    uint32_t buf[100];
    List* list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{3215};
    uint32_t temp2{23587};

    ASSERT_EQ(List_PushFront(list, &temp1), 1);
    ASSERT_EQ(*(uint32_t*)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t*)List_Back(list), temp1);

    ASSERT_EQ(List_PushFront(list, &temp2), 2);
    ASSERT_EQ(*(uint32_t*)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t*)List_Back(list), temp1);
}

TEST(ListTest, MixedPushFrontBack)
{
    uint32_t buf[100];
    List* list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{3215};
    uint32_t temp2{23587};
    uint32_t temp3{555};

    ASSERT_EQ(List_PushFront(list, &temp1), 1);
    ASSERT_EQ(*(uint32_t*)List_Front(list), temp1);
    ASSERT_EQ(*(uint32_t*)List_Back(list), temp1);

    ASSERT_EQ(List_PushFront(list, &temp2), 2);
    ASSERT_EQ(*(uint32_t*)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t*)List_Back(list), temp1);

    ASSERT_EQ(List_PushBack(list, &temp3), 3);
    ASSERT_EQ(*(uint32_t*)List_Front(list), temp2);
    ASSERT_EQ(*(uint32_t*)List_Back(list), temp3);
}

TEST(ListTest, PushFrontIterator)
{
    uint32_t buf[100];
    List* list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{3215};
    uint32_t temp2{23587};

    ASSERT_EQ(List_PushFront(list, &temp1), 1);
    ASSERT_EQ(List_PushFront(list, &temp2), 2);

    ListIterator it = List_Begin(list);
    ListIterator endIt = List_End(list);

    ASSERT_EQ(*(uint32_t*)ListIterator_Value(&it), temp2);
    ListIterator_Increment(&it);
    ASSERT_EQ(*(uint32_t*)ListIterator_Value(&it), temp1);
    ListIterator_Increment(&it);
    ListIterator_Equal(&it, &endIt);
}

TEST(ListTest, PushBackIterator)
{
    uint32_t buf[100];
    List* list = List_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_EQ(List_Size(list), 0);

    uint32_t temp1{3215};
    uint32_t temp2{23587};
    // uint32_t temp3{555};

    ASSERT_EQ(List_PushBack(list, &temp1), 1);
    ASSERT_EQ(List_PushBack(list, &temp2), 2);
    // ASSERT_EQ(List_PushBack(list, &temp3), 3);

    ListIterator it = List_Begin(list);
    ListIterator endIt = List_End(list);

    ASSERT_EQ(*(uint32_t*)ListIterator_Value(&it), temp1);
    ListIterator_Increment(&it);
    ASSERT_EQ(*(uint32_t*)ListIterator_Value(&it), temp2);
    ListIterator_Increment(&it);
    // ASSERT_EQ(*(uint32_t*)ListIterator_Value(&it), temp3);
    // ListIterator_Increment(&it);
    ListIterator_Equal(&it, &endIt);
}
