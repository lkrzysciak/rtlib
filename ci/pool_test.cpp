#include "gtest/gtest.h"
#include "memory/static_pool.h"

declare_static_pool_t(TestPoolType, int, 5);
define_static_pool_t(TestPoolType, int, 5);

TEST(TypedPoolTest, Init)
{
    TestPoolType pool{};
    TestPoolType_Construct(&pool);
}

TEST(TypedPoolTest, Alloc)
{
    TestPoolType pool{};
    TestPoolType_Construct(&pool);
    int* var1{TestPoolType_Allocate(&pool)};
    int* var2{TestPoolType_Allocate(&pool)};
    int* var3{TestPoolType_Allocate(&pool)};
    int* var4{TestPoolType_Allocate(&pool)};
    int* var5{TestPoolType_Allocate(&pool)};
    
    *var1 = 1;
    *var2 = 2;
    *var3 = 3;
    *var4 = 4;
    *var5 = 5;

    ASSERT_EQ(*var1, 1);
    ASSERT_EQ(*var2, 2);
    ASSERT_EQ(*var3, 3);
    ASSERT_EQ(*var4, 4);
    ASSERT_EQ(*var5, 5);    
}

TEST(TypedPoolTest, AllocOverBufferSize)
{
    TestPoolType pool{};
    TestPoolType_Construct(&pool);

    int* var1{TestPoolType_Allocate(&pool)};
    int* var2{TestPoolType_Allocate(&pool)};
    int* var3{TestPoolType_Allocate(&pool)};
    int* var4{TestPoolType_Allocate(&pool)};
    int* var5{TestPoolType_Allocate(&pool)};
    int* var6{TestPoolType_Allocate(&pool)};
    int* var7{TestPoolType_Allocate(&pool)};

     ASSERT_NE(var1, nullptr);
     ASSERT_NE(var2, nullptr);
     ASSERT_NE(var3, nullptr);
     ASSERT_NE(var4, nullptr);
     ASSERT_NE(var5, nullptr);
     ASSERT_EQ(var6, nullptr);
     ASSERT_EQ(var7, nullptr);
}

TEST(TypedPoolTest, AllocFreeAtBufferEnd)
{
    TestPoolType pool{};
    TestPoolType_Construct(&pool);
    int* var1{TestPoolType_Allocate(&pool)};
    int* var2{TestPoolType_Allocate(&pool)};
    int* var3{TestPoolType_Allocate(&pool)};
    int* var4{TestPoolType_Allocate(&pool)};
    int* var5{TestPoolType_Allocate(&pool)};
    
    *var1 = 1;
    *var2 = 2;
    *var3 = 3;
    *var4 = 4;
    *var5 = 5;

    TestPoolType_Release(&pool, var3);
    int* var6{TestPoolType_Allocate(&pool)};
    int* var7{TestPoolType_Allocate(&pool)};

    *var6 = 6;

    ASSERT_EQ(*var1, 1);
    ASSERT_EQ(*var2, 2);
    ASSERT_EQ(*var4, 4);
    ASSERT_EQ(*var5, 5);
    ASSERT_EQ(*var6, 6);    
    ASSERT_EQ(var7, nullptr);    
}

TEST(TypedPoolTest, AllocFreeAtBufferBegin)
{
    TestPoolType pool{};
    TestPoolType_Construct(&pool);
    int* var1{TestPoolType_Allocate(&pool)};
    TestPoolType_Release(&pool, var1);

    int* var2{TestPoolType_Allocate(&pool)};
    int* var3{TestPoolType_Allocate(&pool)};
    int* var4{TestPoolType_Allocate(&pool)};
    int* var5{TestPoolType_Allocate(&pool)};
    int* var6{TestPoolType_Allocate(&pool)};
    int* var7{TestPoolType_Allocate(&pool)};
    
    *var2 = 2;
    *var3 = 3;
    *var4 = 4;
    *var5 = 5;
    *var6 = 6;

    ASSERT_EQ(*var2, 2);
    ASSERT_EQ(*var3, 3);
    ASSERT_EQ(*var4, 4);
    ASSERT_EQ(*var5, 5);
    ASSERT_EQ(*var6, 6);    
    ASSERT_EQ(var7, nullptr);    
}