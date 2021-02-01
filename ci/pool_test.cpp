#include "gtest/gtest.h"

extern "C"
{
#include "memory/dynamic_allocator.h"
#include "memory/custom_allocator_pool.h"
#include "memory/static_pool.h"

    declare_static_pool_t(StaticPoolInt, int, 5);
    define_static_pool_t(StaticPoolInt, int, 5);

    declare_dynamic_allocator_t(PoolDynamicAllocator);
    define_dynamic_allocator_t(PoolDynamicAllocator);
    declare_custom_allocator_pool_t(CustomAllocatorPoolInt, int, PoolDynamicAllocator);
    define_custom_allocator_pool_t(CustomAllocatorPoolInt, int, PoolDynamicAllocator);
}

#define create_wrappers_for_type(Type, MemberType)                                       \
    void Init(Type * const container) { Type##_Construct(container); }                   \
                                                                                         \
    void Deinit(Type * const container) { Type##_Destroy(container); }                   \
                                                                                         \
    MemberType * Allocate(Type * const container) { return Type##_Allocate(container); } \
                                                                                         \
    void Release(Type * const container, MemberType * object) { Type##_Release(container, object); }

create_wrappers_for_type(StaticPoolInt, int);
create_wrappers_for_type(CustomAllocatorPoolInt, int);

template<typename T>
struct PoolTest : public testing::Test
{
    void SetUp() override { Init(&pool); }

    void TearDown() override { Deinit(&pool); }

    T pool;
};

using MyTypes = testing::Types<StaticPoolInt, CustomAllocatorPoolInt>;
TYPED_TEST_SUITE(PoolTest, MyTypes);

TYPED_TEST(PoolTest, Alloc)
{
    int * var1{ Allocate(&this->pool) };
    int * var2{ Allocate(&this->pool) };
    int * var3{ Allocate(&this->pool) };
    int * var4{ Allocate(&this->pool) };
    int * var5{ Allocate(&this->pool) };

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

TYPED_TEST(PoolTest, AllocAndRelease)
{
    int * var1{ Allocate(&this->pool) };
    int * var2{ Allocate(&this->pool) };
    int * var3{ Allocate(&this->pool) };
    int * var4{ Allocate(&this->pool) };
    int * var5{ Allocate(&this->pool) };

    Release(&this->pool, var1);
    Release(&this->pool, var2);
    Release(&this->pool, var3);
    Release(&this->pool, var4);
    Release(&this->pool, var5);
}

TEST(StaticPoolTest, AllocOverBufferSize)
{
    StaticPoolInt pool{};
    Init(&pool);

    int * var1{ Allocate(&pool) };
    int * var2{ Allocate(&pool) };
    int * var3{ Allocate(&pool) };
    int * var4{ Allocate(&pool) };
    int * var5{ Allocate(&pool) };
    int * var6{ Allocate(&pool) };
    int * var7{ Allocate(&pool) };

    ASSERT_NE(var1, nullptr);
    ASSERT_NE(var2, nullptr);
    ASSERT_NE(var3, nullptr);
    ASSERT_NE(var4, nullptr);
    ASSERT_NE(var5, nullptr);
    ASSERT_EQ(var6, nullptr);
    ASSERT_EQ(var7, nullptr);

    Deinit(&pool);
}

TEST(StaticPoolTest, AllocFreeAtBufferEnd)
{
    StaticPoolInt pool{};
    Init(&pool);

    int * var1{ Allocate(&pool) };
    int * var2{ Allocate(&pool) };
    int * var3{ Allocate(&pool) };
    int * var4{ Allocate(&pool) };
    int * var5{ Allocate(&pool) };

    *var1 = 1;
    *var2 = 2;
    *var3 = 3;
    *var4 = 4;
    *var5 = 5;

    Release(&pool, var3);
    int * var6{ Allocate(&pool) };
    int * var7{ Allocate(&pool) };

    *var6 = 6;

    ASSERT_EQ(*var1, 1);
    ASSERT_EQ(*var2, 2);
    ASSERT_EQ(*var4, 4);
    ASSERT_EQ(*var5, 5);
    ASSERT_EQ(*var6, 6);
    ASSERT_EQ(var7, nullptr);

    Deinit(&pool);
}

TEST(StaticPoolTest, AllocFreeAtBufferBegin)
{
    StaticPoolInt pool{};
    Init(&pool);

    int * var1{ Allocate(&pool) };
    Release(&pool, var1);

    int * var2{ Allocate(&pool) };
    int * var3{ Allocate(&pool) };
    int * var4{ Allocate(&pool) };
    int * var5{ Allocate(&pool) };
    int * var6{ Allocate(&pool) };
    int * var7{ Allocate(&pool) };

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

    Deinit(&pool);
}