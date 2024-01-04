#include "gtest/gtest.h"
#include <cstdint>
#include <tuple>
#include "rtlib/vector.h"
#include "rtlib/list.h"
#include "rtlib/deque.h"
#include "rtlib/memory.h"

#define CONTAINER_CAPACITY 100

typedef struct
{
    double doubleVar;
    int intVar;
    bool boolVar;
    uint64_t id;
} StructType;

static int int_Compare(const int * v1, const int * v2)
{
    if(*v1 > *v2)
    {
        return 1;
    }
    else if(*v1 < *v2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int StructType_Compare(const StructType * v1, const StructType * v2)
{
    if(v1->id > v2->id)
    {
        return 1;
    }
    else if(v1->id < v2->id)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

typedef int * IntPtr;

static int IntPtr_Compare(const IntPtr * v1, const IntPtr * v2)
{
    if(*v1 > *v2)
    {
        return 1;
    }
    else if(*v1 < *v2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

dynamic_memory(DynamicAllocator);

static_vector(SVectorWithInt, int, CONTAINER_CAPACITY);
custom_allocator_vector(CVectorWithInt, int, DynamicAllocator);
dynamic_vector(DVectorWithInt, int);
static_vector(SVectorWithPointers, IntPtr, CONTAINER_CAPACITY);
custom_allocator_vector(CVectorWithPointers, IntPtr, DynamicAllocator);
dynamic_vector(DVectorWithPointers, IntPtr);
static_vector(SVectorWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_vector(CVectorWithStruct, StructType, DynamicAllocator);
dynamic_vector(DVectorWithStruct, StructType);

static_deque(SDequeWithInt, int, CONTAINER_CAPACITY);
static_deque(SDequeWithPointers, IntPtr, CONTAINER_CAPACITY);
static_deque(SDequeWithStruct, StructType, CONTAINER_CAPACITY);

static_list(SListWithInt, int, CONTAINER_CAPACITY);
custom_allocator_list(CListWithInt, int, DynamicAllocator);
dynamic_list(DListWithInt, int);
static_list(SListWithPointers, IntPtr, CONTAINER_CAPACITY);
custom_allocator_list(CListWithPointers, IntPtr, DynamicAllocator);
dynamic_list(DListWithPointers, IntPtr);
static_list(SListWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_list(CListWithStruct, StructType, DynamicAllocator);
dynamic_list(DListWithStruct, StructType);

#define create_wrappers_for_type(Type, MemberType)                                     \
                                                                                       \
    void Init(Type * const container)                                                  \
    {                                                                                  \
        Type##_Construct(container);                                                   \
    }                                                                                  \
    void Deinit(Type * const container)                                                \
    {                                                                                  \
        Type##_Destruct(container);                                                    \
    }                                                                                  \
                                                                                       \
    size_t Size(Type * const container)                                                \
    {                                                                                  \
        return Type##_Size(container);                                                 \
    }                                                                                  \
                                                                                       \
    bool Empty(Type * const container)                                                 \
    {                                                                                  \
        return Type##_Empty(container);                                                \
    }                                                                                  \
                                                                                       \
    int PushBack(Type * const container, MemberType value)                             \
    {                                                                                  \
        return Type##_PushBack(container, value);                                      \
    }                                                                                  \
                                                                                       \
    int PushFront(Type * const container, MemberType value)                            \
    {                                                                                  \
        return Type##_PushFront(container, value);                                     \
    }                                                                                  \
                                                                                       \
    int Insert(Type * const container, MemberType value, Type##_Iterator * it)         \
    {                                                                                  \
        return Type##_Insert(container, it, value);                                    \
    }                                                                                  \
    int PopBack(Type * const container)                                                \
    {                                                                                  \
        return Type##_PopBack(container);                                              \
    }                                                                                  \
                                                                                       \
    int PopFront(Type * const container)                                               \
    {                                                                                  \
        return Type##_PopFront(container);                                             \
    }                                                                                  \
                                                                                       \
    int Erase(Type * const container, Type##_Iterator * it)                            \
    {                                                                                  \
        return Type##_Erase(container, it);                                            \
    }                                                                                  \
                                                                                       \
    MemberType Back(Type * const container)                                            \
    {                                                                                  \
        return Type##_Back(container);                                                 \
    }                                                                                  \
                                                                                       \
    MemberType Front(Type * const container)                                           \
    {                                                                                  \
        return Type##_Front(container);                                                \
    }                                                                                  \
                                                                                       \
    auto Begin(Type * const container)                                                 \
    {                                                                                  \
        return Type##_Begin(container);                                                \
    }                                                                                  \
                                                                                       \
    auto End(Type * const container)                                                   \
    {                                                                                  \
        return Type##_End(container);                                                  \
    }                                                                                  \
                                                                                       \
    auto Ref(Type * const container, size_t index)                                     \
    {                                                                                  \
        return Type##_Ref(container, index);                                           \
    }                                                                                  \
                                                                                       \
    auto CRef(const Type * const container, size_t index)                              \
    {                                                                                  \
        return Type##_CRef(container, index);                                          \
    }                                                                                  \
                                                                                       \
    auto Iterator_Ref(Type##_Iterator * const it)                                      \
    {                                                                                  \
        return Type##_Iterator_Ref(it);                                                \
    }                                                                                  \
                                                                                       \
    auto Iterator_CRef(Type##_Iterator * const it)                                     \
    {                                                                                  \
        return Type##_Iterator_CRef(it);                                               \
    }                                                                                  \
                                                                                       \
    void IteratorInc(Type##_Iterator * const it)                                       \
    {                                                                                  \
        return Type##_Iterator_Increment(it);                                          \
    }                                                                                  \
                                                                                       \
    void IteratorDec(Type##_Iterator * const it)                                       \
    {                                                                                  \
        return Type##_Iterator_Decrement(it);                                          \
    }                                                                                  \
                                                                                       \
    bool Iterator_Equal(Type##_Iterator * const first, Type##_Iterator * const second) \
    {                                                                                  \
        return Type##_Iterator_Equal(first, second);                                   \
    }                                                                                  \
                                                                                       \
    auto Find(Type * const container, MemberType value)                                \
    {                                                                                  \
        return Type##_Find(container, value);                                          \
    }                                                                                  \
                                                                                       \
    void Clear(Type * const container)                                                 \
    {                                                                                  \
        Type##_Clear(container);                                                       \
    }

create_wrappers_for_type(SVectorWithInt, int);
create_wrappers_for_type(CVectorWithInt, int);
create_wrappers_for_type(DVectorWithInt, int);
create_wrappers_for_type(SVectorWithPointers, IntPtr);
create_wrappers_for_type(CVectorWithPointers, IntPtr);
create_wrappers_for_type(DVectorWithPointers, IntPtr);
create_wrappers_for_type(SVectorWithStruct, StructType);
create_wrappers_for_type(CVectorWithStruct, StructType);
create_wrappers_for_type(DVectorWithStruct, StructType);

create_wrappers_for_type(SDequeWithInt, int);
create_wrappers_for_type(SDequeWithPointers, IntPtr);
create_wrappers_for_type(SDequeWithStruct, StructType);

create_wrappers_for_type(SListWithInt, int);
create_wrappers_for_type(CListWithInt, int);
create_wrappers_for_type(DListWithInt, int);
create_wrappers_for_type(SListWithPointers, IntPtr);
create_wrappers_for_type(CListWithPointers, IntPtr);
create_wrappers_for_type(DListWithPointers, IntPtr);
create_wrappers_for_type(SListWithStruct, StructType);
create_wrappers_for_type(CListWithStruct, StructType);
create_wrappers_for_type(DListWithStruct, StructType);

template<typename T>
struct ContainerTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct StaticContainerTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct CustomContainerTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct StructTypeTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct QueuePointerTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

using IntTypes = testing::Types<SVectorWithInt, CVectorWithInt, DVectorWithInt, SListWithInt, CListWithInt,
                                DListWithInt, SDequeWithInt>;

using StaticContainerTypes = testing::Types<SVectorWithInt, SListWithInt, SDequeWithInt>;

using CustomContainerTypes = testing::Types<CVectorWithInt, CListWithInt>;

using StructContainerTypes = testing::Types<SVectorWithStruct, CVectorWithStruct, DVectorWithStruct, SListWithStruct,
                                            CListWithStruct, DListWithStruct, SDequeWithStruct>;

using TypesWithPointer = testing::Types<SVectorWithPointers, CVectorWithPointers, DVectorWithPointers,
                                        SListWithPointers, CListWithPointers, DListWithPointers, SDequeWithPointers>;

TYPED_TEST_SUITE(ContainerTest, IntTypes);
TYPED_TEST_SUITE(StaticContainerTest, StaticContainerTypes);
TYPED_TEST_SUITE(CustomContainerTest, CustomContainerTypes);
TYPED_TEST_SUITE(StructTypeTest, StructContainerTypes);
TYPED_TEST_SUITE(QueuePointerTest, TypesWithPointer);

TYPED_TEST(ContainerTest, IsEmptyAfterInit)
{
    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PushBack)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(PushBack(&this->container, temp3), 3);
    auto it = Begin(&this->container);
    ASSERT_EQ(*Iterator_CRef(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp3);

    ASSERT_EQ(Size(&this->container), 3);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PushFront)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    ASSERT_EQ(PushFront(&this->container, temp1), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(PushFront(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp2);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(PushFront(&this->container, temp3), 3);
    auto it = Begin(&this->container);
    ASSERT_EQ(*Iterator_CRef(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp1);

    ASSERT_EQ(Size(&this->container), 3);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, Insert)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto it = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, temp1, &it), 1);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp2, &it), 2);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp3, &it), 3);

    it = Begin(&this->container);
    ASSERT_EQ(*Iterator_CRef(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp2);

    ASSERT_EQ(Size(&this->container), 3);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, Erase)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    auto it = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, temp1, &it), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp2, &it), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Insert(&this->container, temp3, &it), 3);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(*CRef(&this->container, 1), temp3);
    ASSERT_EQ(Back(&this->container), temp2);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&this->container, &it), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&this->container, &it), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    it = Begin(&this->container);
    ASSERT_EQ(Erase(&this->container, &it), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PopBack)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(PopBack(&this->container), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(PopBack(&this->container), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, PopFront)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(PopFront(&this->container), 1);
    ASSERT_EQ(Front(&this->container), temp2);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(PopFront(&this->container), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(ContainerTest, BeginEndIterator)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it       = End(&this->container);

    IteratorDec(&it);

    ASSERT_EQ(*Iterator_CRef(&it_begin), temp1);
    ASSERT_EQ(*Iterator_CRef(&it), temp2);
}

TYPED_TEST(ContainerTest, IteratorIncrementation)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it_end   = End(&this->container);
    auto it       = it_begin;

    ASSERT_EQ(*Iterator_CRef(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp2);
    IteratorInc(&it);
    ASSERT_TRUE(Iterator_Equal(&it, &it_end));
}

TYPED_TEST(ContainerTest, IteratorDecrementation)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it_end   = End(&this->container);

    auto it = it_end;
    IteratorDec(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp2);
    IteratorDec(&it);
    ASSERT_EQ(*Iterator_CRef(&it), temp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}

TYPED_TEST(ContainerTest, IteratorAfterInit)
{
    auto it_begin = Begin(&this->container);
    auto it_end   = End(&this->container);

    ASSERT_TRUE(Iterator_Equal(&it_begin, &it_end));
}

TYPED_TEST(ContainerTest, Iterator_CRefs)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it_end   = End(&this->container);

    auto it = it_end;
    IteratorDec(&it);
    uint32_t newTemp2{ 2468 };
    *Iterator_Ref(&it) = newTemp2;
    ASSERT_EQ(*Iterator_CRef(&it), newTemp2);
    IteratorDec(&it);
    uint32_t newTemp1{ 1357 };
    *Iterator_Ref(&it) = newTemp1;
    ASSERT_EQ(*Iterator_CRef(&it), newTemp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}

TYPED_TEST(ContainerTest, IndexValues)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(*CRef(&this->container, 0), temp1);
    ASSERT_EQ(*CRef(&this->container, 1), temp2);

    uint32_t newTemp1{ 1357 };
    uint32_t newTemp2{ 2468 };

    *Ref(&this->container, 0) = newTemp1;
    ASSERT_EQ(*CRef(&this->container, 0), newTemp1);
    ASSERT_EQ(*CRef(&this->container, 1), temp2);

    *Ref(&this->container, 1) = newTemp2;
    ASSERT_EQ(*CRef(&this->container, 0), newTemp1);
    ASSERT_EQ(*CRef(&this->container, 1), newTemp2);
}

TYPED_TEST(ContainerTest, Ref)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(*Ref(&this->container, 0), temp1);
    ASSERT_EQ(*Ref(&this->container, 1), temp2);

    uint32_t newTemp1{ 1357 };
    uint32_t newTemp2{ 2468 };

    *Ref(&this->container, 0) = newTemp1;
    ASSERT_EQ(*Ref(&this->container, 0), newTemp1);
    ASSERT_EQ(*Ref(&this->container, 1), temp2);

    *Ref(&this->container, 1) = newTemp2;
    ASSERT_EQ(*Ref(&this->container, 0), newTemp1);
    ASSERT_EQ(*Ref(&this->container, 1), newTemp2);
}

TYPED_TEST(ContainerTest, CRef)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(*CRef(&this->container, 0), temp1);
    ASSERT_EQ(*CRef(&this->container, 1), temp2);
}

TYPED_TEST(ContainerTest, FindAllValues)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 582 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);

    auto end_it = End(&this->container);

    auto it_1 = Find(&this->container, temp1);
    ASSERT_EQ(*Iterator_CRef(&it_1), temp1);
    auto it_2 = Find(&this->container, temp2);
    ASSERT_EQ(*Iterator_CRef(&it_2), temp2);
    auto it_3 = Find(&this->container, temp3);
    ASSERT_EQ(*Iterator_CRef(&it_3), temp3);

    auto it_4 = Find(&this->container, temp3 + 1);
    ASSERT_TRUE(Iterator_Equal(&it_4, &end_it));
}

TYPED_TEST(ContainerTest, Permutations)
{
    std::vector<int> testPermutation{
        10,   20,  50,  1,   158, 78,  254, -8,  8756, 51,  4,    5,    1024, 85,    697,
        4587, 123, 258, 741, 963, 951, 843, 628, 762,  384, 6969, 5454, 8514, 74569, 8546
    };

    for(size_t idx = 0; idx < testPermutation.size(); ++idx)
    {
        auto it = Begin(&this->container);
        ASSERT_EQ((idx + 1), Insert(&this->container, testPermutation[idx], &it));
    }

    std::set<int> expectedSet{ testPermutation.begin(), testPermutation.end() };
    std::set<int> receivedSet{};
    auto endIt = End(&this->container);
    for(auto it = Begin(&this->container); !Iterator_Equal(&it, &endIt); IteratorInc(&it))
    {
        receivedSet.insert(*Iterator_CRef(&it));
    }
    ASSERT_EQ(expectedSet, receivedSet);
}

TYPED_TEST(ContainerTest, Fifo)
{
    for(int i = 0; i < 10; i++)
    {
        ASSERT_EQ(PushBack(&this->container, i), i + 1);
    }

    for(int i = 0; i < 10001; i++)
    {
        ASSERT_EQ(Front(&this->container), i);
        ASSERT_EQ(Back(&this->container), i + 9);

        ASSERT_EQ(PushBack(&this->container, i + 10), 11);
        ASSERT_EQ(PopFront(&this->container), 10);
    }
}

TYPED_TEST(ContainerTest, InsertEraseMultipleTimes)
{
    for(int i = 0; i < 10; i++)
    {
        ASSERT_EQ(PushBack(&this->container, i), i + 1);
    }

    ASSERT_EQ(*CRef(&this->container, 0), 0);
    ASSERT_EQ(*CRef(&this->container, 1), 1);
    ASSERT_EQ(*CRef(&this->container, 2), 2);
    ASSERT_EQ(*CRef(&this->container, 3), 3);
    ASSERT_EQ(*CRef(&this->container, 4), 4);
    ASSERT_EQ(*CRef(&this->container, 5), 5);
    ASSERT_EQ(*CRef(&this->container, 6), 6);
    ASSERT_EQ(*CRef(&this->container, 7), 7);
    ASSERT_EQ(*CRef(&this->container, 8), 8);
    ASSERT_EQ(*CRef(&this->container, 9), 9);

    for(int i = 0; i < 10001; i++)
    {
        auto it1 = Begin(&this->container);
        IteratorInc(&it1);
        ASSERT_EQ(Insert(&this->container, i + 10, &it1), 11);

        ASSERT_EQ(*CRef(&this->container, 0), 0);
        ASSERT_EQ(*CRef(&this->container, 1), i + 10);
        ASSERT_EQ(*CRef(&this->container, 2), 1);
        ASSERT_EQ(*CRef(&this->container, 3), 2);
        ASSERT_EQ(*CRef(&this->container, 4), 3);
        ASSERT_EQ(*CRef(&this->container, 5), 4);
        ASSERT_EQ(*CRef(&this->container, 6), 5);
        ASSERT_EQ(*CRef(&this->container, 7), 6);
        ASSERT_EQ(*CRef(&this->container, 8), 7);
        ASSERT_EQ(*CRef(&this->container, 9), 8);
        ASSERT_EQ(*CRef(&this->container, 10), 9);

        auto it2 = Begin(&this->container);
        IteratorInc(&it2);
        ASSERT_EQ(Erase(&this->container, &it2), 10);

        ASSERT_EQ(*CRef(&this->container, 0), 0);
        ASSERT_EQ(*CRef(&this->container, 1), 1);
        ASSERT_EQ(*CRef(&this->container, 2), 2);
        ASSERT_EQ(*CRef(&this->container, 3), 3);
        ASSERT_EQ(*CRef(&this->container, 4), 4);
        ASSERT_EQ(*CRef(&this->container, 5), 5);
        ASSERT_EQ(*CRef(&this->container, 6), 6);
        ASSERT_EQ(*CRef(&this->container, 7), 7);
        ASSERT_EQ(*CRef(&this->container, 8), 8);
        ASSERT_EQ(*CRef(&this->container, 9), 9);
    }
}

TYPED_TEST(ContainerTest, InsertEraseMultipleTimes2)
{
    for(int i = 0; i < 10; i++)
    {
        ASSERT_EQ(PushBack(&this->container, i), i + 1);
    }

    ASSERT_EQ(*CRef(&this->container, 0), 0);
    ASSERT_EQ(*CRef(&this->container, 1), 1);
    ASSERT_EQ(*CRef(&this->container, 2), 2);
    ASSERT_EQ(*CRef(&this->container, 3), 3);
    ASSERT_EQ(*CRef(&this->container, 4), 4);
    ASSERT_EQ(*CRef(&this->container, 5), 5);
    ASSERT_EQ(*CRef(&this->container, 6), 6);
    ASSERT_EQ(*CRef(&this->container, 7), 7);
    ASSERT_EQ(*CRef(&this->container, 8), 8);
    ASSERT_EQ(*CRef(&this->container, 9), 9);

    for(int i = 0; i < 10001; i++)
    {
        auto it1 = End(&this->container);
        IteratorDec(&it1);
        ASSERT_EQ(Insert(&this->container, i + 9, &it1), 11);

        ASSERT_EQ(*CRef(&this->container, 0), 0);  // const
        ASSERT_EQ(*CRef(&this->container, 1), i + 1);
        ASSERT_EQ(*CRef(&this->container, 2), i + 2);
        ASSERT_EQ(*CRef(&this->container, 3), i + 3);
        ASSERT_EQ(*CRef(&this->container, 4), i + 4);
        ASSERT_EQ(*CRef(&this->container, 5), i + 5);
        ASSERT_EQ(*CRef(&this->container, 6), i + 6);
        ASSERT_EQ(*CRef(&this->container, 7), i + 7);
        ASSERT_EQ(*CRef(&this->container, 8), i + 8);
        ASSERT_EQ(*CRef(&this->container, 9), i + 9);
        ASSERT_EQ(*CRef(&this->container, 10), 9);  // const

        auto it2 = Begin(&this->container);
        IteratorInc(&it2);
        ASSERT_EQ(Erase(&this->container, &it2), 10);

        ASSERT_EQ(*CRef(&this->container, 0), 0);  // const
        ASSERT_EQ(*CRef(&this->container, 1), i + 2);
        ASSERT_EQ(*CRef(&this->container, 2), i + 3);
        ASSERT_EQ(*CRef(&this->container, 3), i + 4);
        ASSERT_EQ(*CRef(&this->container, 4), i + 5);
        ASSERT_EQ(*CRef(&this->container, 5), i + 6);
        ASSERT_EQ(*CRef(&this->container, 6), i + 7);
        ASSERT_EQ(*CRef(&this->container, 7), i + 8);
        ASSERT_EQ(*CRef(&this->container, 8), i + 9);
        ASSERT_EQ(*CRef(&this->container, 9), 9);  // const
    }
}

TYPED_TEST(ContainerTest, InsertMultipleTimes)
{
    for(int i = 0; i < 10; i++)
    {
        ASSERT_EQ(PushBack(&this->container, i), i + 1);
    }

    ASSERT_EQ(*CRef(&this->container, 0), 0);
    ASSERT_EQ(*CRef(&this->container, 1), 1);
    ASSERT_EQ(*CRef(&this->container, 2), 2);
    ASSERT_EQ(*CRef(&this->container, 3), 3);
    ASSERT_EQ(*CRef(&this->container, 4), 4);
    ASSERT_EQ(*CRef(&this->container, 5), 5);
    ASSERT_EQ(*CRef(&this->container, 6), 6);
    ASSERT_EQ(*CRef(&this->container, 7), 7);
    ASSERT_EQ(*CRef(&this->container, 8), 8);
    ASSERT_EQ(*CRef(&this->container, 9), 9);

    for(int i = 0; i < 10001; i++)
    {
        auto it1 = End(&this->container);
        IteratorDec(&it1);
        ASSERT_EQ(Insert(&this->container, i + 9, &it1), 11);

        ASSERT_EQ(*CRef(&this->container, 0), i + 0);
        ASSERT_EQ(*CRef(&this->container, 1), i + 1);
        ASSERT_EQ(*CRef(&this->container, 2), i + 2);
        ASSERT_EQ(*CRef(&this->container, 3), i + 3);
        ASSERT_EQ(*CRef(&this->container, 4), i + 4);
        ASSERT_EQ(*CRef(&this->container, 5), i + 5);
        ASSERT_EQ(*CRef(&this->container, 6), i + 6);
        ASSERT_EQ(*CRef(&this->container, 7), i + 7);
        ASSERT_EQ(*CRef(&this->container, 8), i + 8);
        ASSERT_EQ(*CRef(&this->container, 9), i + 9);
        ASSERT_EQ(*CRef(&this->container, 10), 9);  // const

        PopFront(&this->container);

        ASSERT_EQ(*CRef(&this->container, 0), i + 1);
        ASSERT_EQ(*CRef(&this->container, 1), i + 2);
        ASSERT_EQ(*CRef(&this->container, 2), i + 3);
        ASSERT_EQ(*CRef(&this->container, 3), i + 4);
        ASSERT_EQ(*CRef(&this->container, 4), i + 5);
        ASSERT_EQ(*CRef(&this->container, 5), i + 6);
        ASSERT_EQ(*CRef(&this->container, 6), i + 7);
        ASSERT_EQ(*CRef(&this->container, 7), i + 8);
        ASSERT_EQ(*CRef(&this->container, 8), i + 9);
        ASSERT_EQ(*CRef(&this->container, 9), 9);  // const
    }
}

TYPED_TEST(ContainerTest, Clear)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);
    ASSERT_EQ(Size(&this->container), 3);

    Clear(&this->container);

    ASSERT_EQ(Size(&this->container), 0);
}

TYPED_TEST(StaticContainerTest, PushBackOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushBack(&this->container, temp1), i + 1);
    }
    ASSERT_EQ(PushBack(&this->container, temp1), ALLOCATION_ERROR);
}

TYPED_TEST(StaticContainerTest, PushFrontOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushFront(&this->container, temp1), i + 1);
    }
    ASSERT_EQ(PushFront(&this->container, temp1), ALLOCATION_ERROR);
}

TYPED_TEST(StaticContainerTest, InsertOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        auto it = Begin(&this->container);
        ASSERT_EQ(Insert(&this->container, temp1, &it), i + 1);
    }
    auto it = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, temp1, &it), ALLOCATION_ERROR);
}

TYPED_TEST(CustomContainerTest, AddALotOfElementsToMakeManyReallocations)
{
    uint32_t temp1{ 3215 };

    for(int i = 0; i < 1000; ++i)
    {
        auto it = Begin(&this->container);
        ASSERT_EQ(Insert(&this->container, temp1 + i, &it), i + 1);
        ASSERT_EQ(Front(&this->container), temp1 + i);
    }
}

TYPED_TEST(StructTypeTest, StructMembersPushBack)
{
    StructType var1 = { .doubleVar = 159.753, .intVar = 0x12345678, .boolVar = true, .id = 0x1 };
    StructType var2 = { .doubleVar = 0.569, .intVar = 0x1, .boolVar = false, .id = 0xffffffffffffffff };

    ASSERT_EQ(PushBack(&this->container, var1), 1);
    ASSERT_EQ(PushBack(&this->container, var2), 2);

    auto it = Begin(&this->container);

    auto receivedVar1 = *Iterator_CRef(&it);
    ASSERT_DOUBLE_EQ(var1.doubleVar, receivedVar1.doubleVar);
    ASSERT_EQ(var1.intVar, receivedVar1.intVar);
    ASSERT_EQ(var1.boolVar, receivedVar1.boolVar);
    ASSERT_EQ(var1.id, receivedVar1.id);

    IteratorInc(&it);
    auto receivedVar2 = *Iterator_CRef(&it);
    ASSERT_DOUBLE_EQ(var2.doubleVar, receivedVar2.doubleVar);
    ASSERT_EQ(var2.intVar, receivedVar2.intVar);
    ASSERT_EQ(var2.boolVar, receivedVar2.boolVar);
    ASSERT_EQ(var2.id, receivedVar2.id);
}

TYPED_TEST(StructTypeTest, StructMembersPushFront)
{
    StructType var1 = { .doubleVar = 159.753, .intVar = 0x12345678, .boolVar = true, .id = 0x1 };
    StructType var2 = { .doubleVar = 0.569, .intVar = 0x1, .boolVar = false, .id = 0xffffffffffffffff };

    ASSERT_EQ(PushFront(&this->container, var1), 1);
    ASSERT_EQ(PushFront(&this->container, var2), 2);

    auto it = Begin(&this->container);

    auto receivedVar2 = *Iterator_CRef(&it);
    ASSERT_DOUBLE_EQ(var2.doubleVar, receivedVar2.doubleVar);
    ASSERT_EQ(var2.intVar, receivedVar2.intVar);
    ASSERT_EQ(var2.boolVar, receivedVar2.boolVar);
    ASSERT_EQ(var2.id, receivedVar2.id);

    IteratorInc(&it);
    auto receivedVar1 = *Iterator_CRef(&it);
    ASSERT_DOUBLE_EQ(var1.doubleVar, receivedVar1.doubleVar);
    ASSERT_EQ(var1.intVar, receivedVar1.intVar);
    ASSERT_EQ(var1.boolVar, receivedVar1.boolVar);
    ASSERT_EQ(var1.id, receivedVar1.id);
}

TYPED_TEST(StructTypeTest, StructMembersInsert)
{
    StructType var1 = { .doubleVar = 159.753, .intVar = 0x12345678, .boolVar = true, .id = 0x1 };
    StructType var2 = { .doubleVar = 0.569, .intVar = 0x1, .boolVar = false, .id = 0xffffffffffffffff };

    auto begin = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, var1, &begin), 1);
    begin = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, var2, &begin), 2);

    auto it = Begin(&this->container);

    auto receivedVar2 = *Iterator_CRef(&it);
    ASSERT_DOUBLE_EQ(var2.doubleVar, receivedVar2.doubleVar);
    ASSERT_EQ(var2.intVar, receivedVar2.intVar);
    ASSERT_EQ(var2.boolVar, receivedVar2.boolVar);
    ASSERT_EQ(var2.id, receivedVar2.id);

    IteratorInc(&it);
    auto receivedVar1 = *Iterator_CRef(&it);
    ASSERT_DOUBLE_EQ(var1.doubleVar, receivedVar1.doubleVar);
    ASSERT_EQ(var1.intVar, receivedVar1.intVar);
    ASSERT_EQ(var1.boolVar, receivedVar1.boolVar);
    ASSERT_EQ(var1.id, receivedVar1.id);
}

TYPED_TEST(QueuePointerTest, Insert)
{
    int * a = new int(5);
    int * b = new int(11);

    ASSERT_EQ(PushBack(&this->container, a), 1);
    ASSERT_EQ(PushBack(&this->container, b), 2);

    auto it = Begin(&this->container);
    ASSERT_EQ(a, *Iterator_CRef(&it));
    IteratorInc(&it);
    ASSERT_EQ(b, *Iterator_CRef(&it));

    Clear(&this->container);

    delete a;
    delete b;
}

// must to at the end to verify if we have valid declarations
dynamic_memory_impl(DynamicAllocator);

static_vector_impl(SVectorWithInt, int, CONTAINER_CAPACITY);
custom_allocator_vector_impl(CVectorWithInt, int, DynamicAllocator);
dynamic_vector_impl(DVectorWithInt, int);
static_vector_impl(SVectorWithPointers, IntPtr, CONTAINER_CAPACITY);
custom_allocator_vector_impl(CVectorWithPointers, IntPtr, DynamicAllocator);
dynamic_vector_impl(DVectorWithPointers, IntPtr);
static_vector_impl(SVectorWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_vector_impl(CVectorWithStruct, StructType, DynamicAllocator);
dynamic_vector_impl(DVectorWithStruct, StructType);

static_deque_impl(SDequeWithInt, int, CONTAINER_CAPACITY);
static_deque_impl(SDequeWithPointers, IntPtr, CONTAINER_CAPACITY);
static_deque_impl(SDequeWithStruct, StructType, CONTAINER_CAPACITY);

static_list_impl(SListWithInt, int, CONTAINER_CAPACITY);
custom_allocator_list_impl(CListWithInt, int, DynamicAllocator);
dynamic_list_impl(DListWithInt, int);
static_list_impl(SListWithPointers, IntPtr, CONTAINER_CAPACITY);
custom_allocator_list_impl(CListWithPointers, IntPtr, DynamicAllocator);
dynamic_list_impl(DListWithPointers, IntPtr);
static_list_impl(SListWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_list_impl(CListWithStruct, StructType, DynamicAllocator);
dynamic_list_impl(DListWithStruct, StructType);
