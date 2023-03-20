#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdint>
#include <tuple>
#include "rtlib/event_loop.h"

#define CONTAINER_CAPACITY 100

extern "C"
{
    memory_t(EventLoopAllocator);
    dynamic_memory_t(EventLoopAllocator);

    event_loop_t(StaticEventLoopVal, int);
    static_event_loop_t(StaticEventLoopVal, int, CONTAINER_CAPACITY);
    event_loop_t(CustomAllocatorEventLoopVal, int);
    custom_allocator_event_loop_t(CustomAllocatorEventLoopVal, int, EventLoopAllocator);
    event_loop_t(DynamicEventLoopVal, int);
    dynamic_event_loop_t(DynamicEventLoopVal, int);

    event_loop_t(StaticEventLoopPtr, int);
    static_event_loop_t(StaticEventLoopPtr, int, CONTAINER_CAPACITY);
    event_loop_t(CustomAllocatorEventLoopPtr, int);
    custom_allocator_event_loop_t(CustomAllocatorEventLoopPtr, int, EventLoopAllocator);
    event_loop_t(DynamicEventLoopPtr, int);
    dynamic_event_loop_t(DynamicEventLoopPtr, int);
}

#define create_wrappers_for_type(Type, MemberType)                                      \
                                                                                        \
    void Construct(Type * const container)                                              \
    {                                                                                   \
        Type##_Construct(container);                                                    \
    }                                                                                   \
                                                                                        \
    void Destroy(Type * const container)                                                \
    {                                                                                   \
        Type##_Destroy(container);                                                      \
    }                                                                                   \
                                                                                        \
    size_t Size(Type * const container)                                                 \
    {                                                                                   \
        return Type##_Size(container);                                                  \
    }                                                                                   \
                                                                                        \
    int Enqueue(Type * const container, void (*callback)(MemberType), MemberType value) \
    {                                                                                   \
        return Type##_Enqueue(container, callback, value);                              \
    }                                                                                   \
                                                                                        \
    int Process(Type * const container)                                                 \
    {                                                                                   \
        return Type##_Process(container);                                               \
    }

struct TestIntMock
{
    MOCK_METHOD1(call, void(int));
};

static TestIntMock * mock;

static void test_int_val(int val)
{
    mock->call(val);
}

create_wrappers_for_type(StaticEventLoopVal, int);
create_wrappers_for_type(CustomAllocatorEventLoopVal, int);
create_wrappers_for_type(DynamicEventLoopVal, int);
create_wrappers_for_type(StaticEventLoopPtr, int);
create_wrappers_for_type(CustomAllocatorEventLoopPtr, int);
create_wrappers_for_type(DynamicEventLoopPtr, int);

template<typename T>
struct EventLoopTest : public testing::Test
{
    void SetUp() override
    {
        mock = new TestIntMock();
        Construct(&container);
    }

    void TearDown() override
    {
        Destroy(&container);
        delete mock;
        mock = NULL;
    }

    T container;
};

using ValTypes = testing::Types<StaticEventLoopVal, CustomAllocatorEventLoopVal, DynamicEventLoopVal>;

TYPED_TEST_SUITE(EventLoopTest, ValTypes);

TYPED_TEST(EventLoopTest, IsEmptyAfterInit)
{
    ASSERT_EQ(Size(&this->container), 0);
}

TYPED_TEST(EventLoopTest, Process)
{
    Enqueue(&this->container, test_int_val, 2);
    Enqueue(&this->container, test_int_val, 4);
    Enqueue(&this->container, test_int_val, 6);
    Enqueue(&this->container, test_int_val, 1);

    EXPECT_CALL(*mock, call(2)).Times(1);
    Process(&this->container);

    EXPECT_CALL(*mock, call(4)).Times(1);
    Process(&this->container);

    EXPECT_CALL(*mock, call(6)).Times(1);
    Process(&this->container);

    EXPECT_CALL(*mock, call(1)).Times(1);
    Process(&this->container);

    Process(&this->container);
}
