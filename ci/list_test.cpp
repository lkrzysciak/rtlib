#include "gtest/gtest.h"
#include "containers/_list.h"
#include <cstdint>


TEST(ListTest, Init)
{
    uint32_t buf[100];
    _List* list_1 = _List_Init(sizeof(uint32_t), buf, sizeof(buf));

    ASSERT_EQ(_List_Capacity(list_1), (sizeof(buf) - _List_HeaderSize()) / sizeof(buf[0]));
}

TEST(ListTest, NotEnoughSpaceToInit)
{
    uint8_t buf1[_List_HeaderSize()];
    uint8_t buf2[_List_HeaderSize() - 1];
    _List* list_1 = _List_Init(sizeof(uint8_t), buf1, sizeof(buf1));
    _List* list_2 = _List_Init(sizeof(uint8_t), buf2, sizeof(buf2));
    ASSERT_EQ((void*)list_1, (void*)buf1);
    ASSERT_EQ(list_2, nullptr);
}

TEST(ListTest, SizeAfterInit)
{
    uint8_t buf1[_List_HeaderSize()];
    _List* list = _List_Init(sizeof(uint8_t), buf1, sizeof(buf1));
    ASSERT_EQ(_List_Size(list), 0);
}
