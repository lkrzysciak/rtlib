#include "gtest/gtest.h"
#include "for_tests/object_pool.h"
#include <cstdint>

extern size_t Sizeof_ObjectBlock;
extern size_t Sizeof_Pool;

TEST(PoolTest, Init)
{
    uint32_t buf[100];
    Pool * pool = Pool_Init(buf, sizeof(buf), sizeof(uint32_t));
    ASSERT_NE(pool, nullptr);
}

TEST(PoolTest, InitWithTooSmallBuffer)
{
    uint8_t buf1[Sizeof_Pool - 1];
    uint8_t buf2[Sizeof_Pool];

    EXPECT_DEATH(Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t)), "");
    EXPECT_NE(Pool_Init(buf2, sizeof(buf2), sizeof(uint32_t)), nullptr);
}

TEST(PoolTest, Capacity)
{
    size_t buf1Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 4 - 1 };
     size_t buf2Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 4 };
    uint8_t buf1[buf1Size + Sizeof_Pool];
    uint8_t buf2[buf2Size + Sizeof_Pool];
    const size_t buf1Capacity{ buf1Size / (sizeof(uint32_t) + Sizeof_ObjectBlock) };
    const size_t buf2Capacity{ buf2Size / (sizeof(uint32_t) + Sizeof_ObjectBlock) };
    ASSERT_GT(buf2Capacity, buf1Capacity);

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    Pool * pool2 = Pool_Init(buf2, sizeof(buf2), sizeof(uint32_t));

    ASSERT_EQ(Pool_Capacity(pool1), buf1Capacity);
    ASSERT_EQ(Pool_Capacity(pool2), buf2Capacity);
}

TEST(PoolTest, AllocOnce)
{
    constexpr size_t buf1Size{ 99 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));

    ASSERT_EQ(Pool_Alloc(pool1), buf1 + Sizeof_Pool + Sizeof_ObjectBlock);
}

TEST(PoolTest, MultipleAlloc)
{
    constexpr size_t buf1Size{ 99 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));

    uint8_t * ptr1 = buf1 + Sizeof_Pool + Sizeof_ObjectBlock;
    ASSERT_EQ(Pool_Alloc(pool1), ptr1);
    uint8_t * ptr2 = ptr1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    ASSERT_EQ(Pool_Alloc(pool1), ptr2);
    uint8_t * ptr3 = ptr2 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    ASSERT_EQ(Pool_Alloc(pool1), ptr3);
}

TEST(PoolTest, NotEnoughSpaceToAlloc)
{
    size_t buf1Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 3 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    size_t buf2Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 3 - 1 };
    uint8_t buf2[buf2Size + Sizeof_Pool];

    Pool * pool2 = Pool_Init(buf2, sizeof(buf2), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool2), 2);

    uint8_t * ptr1_1 = buf1 + Sizeof_Pool + Sizeof_ObjectBlock;
    uint8_t * ptr1_2 = buf2 + Sizeof_Pool + Sizeof_ObjectBlock;
    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool2), ptr1_2);

    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    uint8_t * ptr2_2 = ptr1_2 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool2), ptr2_2);

    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool2), nullptr);

    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
    ASSERT_EQ(Pool_Alloc(pool2), nullptr);
}

TEST(PoolTest, AllocAndFreeBlock)
{
    size_t buf1Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 3 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + Sizeof_Pool + Sizeof_ObjectBlock;
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    // uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + sizeof(ObjectBlock);

    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    Pool_Free(pool1, ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
}

TEST(PoolTest, AllocFreeBlockAndFillBuffer)
{
    size_t buf1Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 3 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + Sizeof_Pool + Sizeof_ObjectBlock;
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;

    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    Pool_Free(pool1, ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
}

TEST(PoolTest, FillBufferAndFreeAloccBlock)
{
    size_t buf1Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 3 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + Sizeof_Pool + Sizeof_ObjectBlock;
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;

    ASSERT_EQ(Pool_Alloc(pool1), ptr1_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr3_1);
    ASSERT_EQ(Pool_Alloc(pool1), nullptr);
    Pool_Free(pool1, ptr2_1);
    ASSERT_EQ(Pool_Alloc(pool1), ptr2_1);
}

TEST(PoolTest, AllocationQueue)
{
    size_t buf1Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 3 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + Sizeof_Pool + Sizeof_ObjectBlock;
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;

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
    size_t buf1Size{ (sizeof(uint32_t) + Sizeof_ObjectBlock) * 3 };
    uint8_t buf1[buf1Size + Sizeof_Pool];

    Pool * pool1 = Pool_Init(buf1, sizeof(buf1), sizeof(uint32_t));
    ASSERT_EQ(Pool_Capacity(pool1), 3);

    uint8_t * ptr1_1 = buf1 + Sizeof_Pool + Sizeof_ObjectBlock;
    uint8_t * ptr2_1 = ptr1_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;
    uint8_t * ptr3_1 = ptr2_1 + sizeof(uint32_t) + Sizeof_ObjectBlock;

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
