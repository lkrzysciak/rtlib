#include "gtest/gtest.h"
#include "memory/object_pool.h"
#include "memory/object_pool.c"
#include <cstdint>

TEST(PoolTest, Init)
{
    uint32_t buf[100];
    Pool * pool = Pool_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_NE(pool, nullptr);
}

TEST(PoolTest, InitWithTooSmallBuffer)
{
    uint8_t buf1[sizeof(Pool) - 1];
    uint8_t buf2[sizeof(Pool)];

    EXPECT_DEATH(Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t)), "");
    EXPECT_NE(Pool_Init(buf2, sizeof(buf2), sizeof(uint32_t)), nullptr);
}

TEST(PoolTest, Capacity)
{
    constexpr size_t buf1Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 4 - 1 };
    constexpr size_t buf2Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 4 };
    uint8_t buf1[buf1Size + sizeof(Pool)];
    uint8_t buf2[buf2Size + sizeof(Pool)];
    const size_t buf1Capacity{ buf1Size / (sizeof(uint32_t) + sizeof(ObjectBlock)) };
    const size_t buf2Capacity{ buf2Size / (sizeof(uint32_t) + sizeof(ObjectBlock)) };
    ASSERT_GT(buf2Capacity, buf1Capacity);

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    Pool * pool2 = Pool_Init(buf2, sizeof(buf2), sizeof(uint32_t));

    ASSERT_EQ(Pool_Capacity(pool1), buf1Capacity);
    ASSERT_EQ(Pool_Capacity(pool2), buf2Capacity);
}

TEST(PoolTest, AllocOnce)
{
    constexpr size_t buf1Size{ 99 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));

    ASSERT_EQ(Pool_Alloc(pool1), buf1 + sizeof(Pool) + sizeof(ObjectBlock));
}

TEST(PoolTest, MultipleAlloc)
{
    constexpr size_t buf1Size{ 99 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));

    uint8_t * ptr1 = buf1 + sizeof(Pool) + sizeof(ObjectBlock);
    ASSERT_EQ(Pool_Alloc(pool1), ptr1);
    uint8_t * ptr2 = ptr1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2);
    uint8_t * ptr3 = ptr2 + sizeof(uint32_t) + sizeof(ObjectBlock);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3);
}

TEST(PoolTest, NotEnoughSpaceToAlloc)
{
    constexpr size_t buf1Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 3 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    constexpr size_t buf2Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 3 - 1 };
    uint8_t buf2[buf2Size + sizeof(Pool)];

    Pool * pool2 = Pool_Init(buf2, sizeof(buf2), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool2), 2);

    uint8_t * ptr1_1 = buf1 + sizeof(Pool) + sizeof(ObjectBlock);
    uint8_t * ptr1_2 = buf2 + sizeof(Pool) + sizeof(ObjectBlock);
    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool2), ptr1_2);

    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    uint8_t * ptr2_2 = ptr1_2 + sizeof(uint32_t) + sizeof(ObjectBlock);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool2), ptr2_2);

    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool2), nullptr);

    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
    ASSERT_EQ(Pool_Alloc(pool2), nullptr);
}

TEST(PoolTest, AllocAndFreeBlock)
{
    constexpr size_t buf1Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 3 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + sizeof(Pool) + sizeof(ObjectBlock);
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + sizeof(ObjectBlock);

    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    Pool_Free(pool1, ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
}

TEST(PoolTest, AllocFreeBlockAndFillBuffer)
{
    constexpr size_t buf1Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 3 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + sizeof(Pool) + sizeof(ObjectBlock);
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + sizeof(ObjectBlock);

    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    Pool_Free(pool1, ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
}

TEST(PoolTest, FillBufferAndFreeAloccBlock)
{
    constexpr size_t buf1Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 3 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + sizeof(Pool) + sizeof(ObjectBlock);
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + sizeof(ObjectBlock);

    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
    Pool_Free(pool1, ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
}

TEST(PoolTest, AllocationQueue)
{
    constexpr size_t buf1Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 3 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + sizeof(Pool) + sizeof(ObjectBlock);
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + sizeof(ObjectBlock);

    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
    Pool_Free(pool1, ptr3_1);
    Pool_Free(pool1, ptr2_1);
    Pool_Free(pool1, ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
}

TEST(PoolTest, Size)
{
    constexpr size_t buf1Size{ (sizeof(uint32_t) + sizeof(ObjectBlock)) * 3 };
    uint8_t buf1[buf1Size + sizeof(Pool)];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + sizeof(Pool) + sizeof(ObjectBlock);
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + sizeof(ObjectBlock);
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + sizeof(ObjectBlock);

    ASSERT_EQ(Pool_Size(pool1), 0);
    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Size(pool1), 1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Size(pool1), 2);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Size(pool1), 3);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
    ASSERT_EQ(Pool_Size(pool1), 3);
    Pool_Free(pool1, ptr3_1);
    ASSERT_EQ(Pool_Size(pool1), 2);
    Pool_Free(pool1, ptr2_1);
    ASSERT_EQ(Pool_Size(pool1), 1);
    Pool_Free(pool1, ptr1_1);
    ASSERT_EQ(Pool_Size(pool1), 0);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Size(pool1), 1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Size(pool1), 2);
    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Size(pool1), 3);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
    ASSERT_EQ(Pool_Size(pool1), 3);
}