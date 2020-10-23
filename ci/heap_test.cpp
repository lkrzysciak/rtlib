#include "gtest/gtest.h"
#include <cstdint>

#include "for_tests/heap.h"
#include "for_tests/heap.c"  // To have access to private members

TEST(HeapTest, Init)
{
    uint8_t buf[100];

    Heap * heap = Heap_Init(buf, sizeof(buf));
    ASSERT_NE(heap, nullptr);
}

TEST(HeapTest, InitWithTooSmallBuffer)
{
    uint8_t buf[sizeof(Heap) + sizeof(MemoryBlock) - 1];

    EXPECT_DEATH(Heap_Init(buf, sizeof(buf)), "");
}

TEST(HeapTest, AllocateOnce)
{
    int8_t buf[100];

    Heap * heap = Heap_Init(buf, sizeof(buf));

    void * ptr1 = Heap_Alloc(heap, 10);
    ASSERT_EQ(ptr1, buf + sizeof(Heap) + sizeof(MemoryBlock));
}

// TEST(HeapTest, AllocateTwice)
// {
//     int8_t buf[100];

//     Heap* heap = Heap_Init(buf, sizeof(buf));

//     void* ptr1 = Heap_Alloc(heap, 10);
//     ASSERT_EQ(ptr1, buf + sizeof(Heap) + sizeof(MemoryBlock));
//     void* ptr2 = Heap_Alloc(heap, 10);
//     ASSERT_EQ(ptr2, buf + sizeof(Heap) + sizeof(MemoryBlock) + 10 + sizeof(MemoryBlock));
// }
