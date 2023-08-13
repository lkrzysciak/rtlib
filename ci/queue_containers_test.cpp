#include "gtest/gtest.h"
#include <cstdint>
#include <tuple>
#include "rtlib/vector.h"
#include "rtlib/list.h"
#include "rtlib/deque.h"
#include "rtlib/memory.h"

#define CONTAINER_CAPACITY 100

extern "C"
{
    vector_t(VectorTestType, int);
    static_vector_t(VectorTestType, int, CONTAINER_CAPACITY);
    list_t(ListTestType, int);
    static_list_t(ListTestType, int, CONTAINER_CAPACITY);
    deque_t(DequeTestType, int);
    static_deque_t(DequeTestType, int, CONTAINER_CAPACITY);

    memory_t(DynamicAllocator);
    dynamic_memory_t(DynamicAllocator);
    vector_t(CustomAllocatorVector, int);
    custom_allocator_vector_t(CustomAllocatorVector, int, DynamicAllocator);
    list_t(CustomAllocatorList, int);
    custom_allocator_list_t(CustomAllocatorList, int, DynamicAllocator);

    vector_t(SVectorWithPointers, int *);
    static_vector_t(SVectorWithPointers, int *, CONTAINER_CAPACITY);
    list_t(SListWithPointers, int *);
    static_list_t(SListWithPointers, int *, CONTAINER_CAPACITY);
    vector_t(CVectorWithPointers, int *);
    custom_allocator_vector_t(CVectorWithPointers, int *, DynamicAllocator);
    list_t(CListWithPointers, int *);
    custom_allocator_list_t(CListWithPointers, int *, DynamicAllocator);

    vector_t(DynamicVector, int);
    dynamic_vector_t(DynamicVector, int);
    list_t(DynamicList, int);
    dynamic_list_t(DynamicList, int);

    typedef struct
    {
        double doubleVar;
        int intVar;
        bool boolVar;
        uint64_t id;
    } StructType;

    vector_t(StructTypeStaticVector, StructType);
    static_vector_t(StructTypeStaticVector, StructType, CONTAINER_CAPACITY);
    list_t(StructTypeStaticList, StructType);
    static_list_t(StructTypeStaticList, StructType, CONTAINER_CAPACITY);
    vector_t(StructTypeDynamicVector, StructType);
    dynamic_vector_t(StructTypeDynamicVector, StructType);
    list_t(StructTypeDynamicList, StructType);
    dynamic_list_t(StructTypeDynamicList, StructType);
}

// v3 api
static_vector(VectorTestTypeV3, int, CONTAINER_CAPACITY);
custom_allocator_vector(CustomAllocatorVectorV3, int, DynamicAllocator);
dynamic_vector(DynamicVectorV3, int);

static_vector(VectorTestTypeV3NullCmp, int, CONTAINER_CAPACITY);
custom_allocator_vector(CustomAllocatorVectorV3NullCmp, int, DynamicAllocator);
dynamic_vector(DynamicVectorV3NullCmp, int);

static int compare_set_ints(const int * v1, const int * v2)
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

static int compare_set_ints_ptr(const int ** v1, const int ** v2)
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

static int compare_custom_ints(const int * v1, const int * v2)
{
    /* We are looking for x+1 value - for test only */
    if(*v1 > (*v2 + 1))
    {
        return 1;
    }
    else if(*v1 < (*v2 + 1))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int compare_struct_type(const StructType * v1, const StructType * v2)
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

#define create_wrappers_for_type(Type, CompareFunction, MemberType)                               \
                                                                                                  \
    void Init(Type * const container)                                                             \
    {                                                                                             \
        Type##_Construct(container, CompareFunction);                                             \
    }                                                                                             \
    void Deinit(Type * const container)                                                           \
    {                                                                                             \
        Type##_Destroy(container);                                                                \
    }                                                                                             \
                                                                                                  \
    size_t Size(Type * const container)                                                           \
    {                                                                                             \
        return Type##_Size(container);                                                            \
    }                                                                                             \
                                                                                                  \
    bool Empty(Type * const container)                                                            \
    {                                                                                             \
        return Type##_Empty(container);                                                           \
    }                                                                                             \
                                                                                                  \
    int PushBack(Type * const container, MemberType value)                                        \
    {                                                                                             \
        return Type##_PushBack(container, value);                                                 \
    }                                                                                             \
                                                                                                  \
    int PushFront(Type * const container, MemberType value)                                       \
    {                                                                                             \
        return Type##_PushFront(container, value);                                                \
    }                                                                                             \
                                                                                                  \
    int Insert(Type * const container, MemberType value, Type##_Iterator * it)                    \
    {                                                                                             \
        return Type##_Insert(container, it, value);                                               \
    }                                                                                             \
    int PopBack(Type * const container)                                                           \
    {                                                                                             \
        return Type##_PopBack(container);                                                         \
    }                                                                                             \
                                                                                                  \
    int PopFront(Type * const container)                                                          \
    {                                                                                             \
        return Type##_PopFront(container);                                                        \
    }                                                                                             \
                                                                                                  \
    int Erase(Type * const container, Type##_Iterator * it)                                       \
    {                                                                                             \
        return Type##_Erase(container, it);                                                       \
    }                                                                                             \
                                                                                                  \
    MemberType Back(Type * const container)                                                       \
    {                                                                                             \
        return Type##_Back(container);                                                            \
    }                                                                                             \
                                                                                                  \
    MemberType Front(Type * const container)                                                      \
    {                                                                                             \
        return Type##_Front(container);                                                           \
    }                                                                                             \
                                                                                                  \
    auto Begin(Type * const container)                                                            \
    {                                                                                             \
        return Type##_Begin(container);                                                           \
    }                                                                                             \
                                                                                                  \
    auto End(Type * const container)                                                              \
    {                                                                                             \
        return Type##_End(container);                                                             \
    }                                                                                             \
                                                                                                  \
    auto GetValue(Type * const container, size_t index)                                           \
    {                                                                                             \
        return Type##_GetValue(container, index);                                                 \
    }                                                                                             \
                                                                                                  \
    void SetValue(Type * const container, size_t index, MemberType value)                         \
    {                                                                                             \
        Type##_SetValue(container, index, value);                                                 \
    }                                                                                             \
                                                                                                  \
    auto Ref(Type * const container, size_t index)                                                \
    {                                                                                             \
        return Type##_Ref(container, index);                                                      \
    }                                                                                             \
                                                                                                  \
    auto CRef(const Type * const container, size_t index)                                         \
    {                                                                                             \
        return Type##_CRef(container, index);                                                     \
    }                                                                                             \
                                                                                                  \
    auto IteratorValue(Type##_Iterator * const it)                                                \
    {                                                                                             \
        return Type##_Iterator_GetValue(it);                                                      \
    }                                                                                             \
                                                                                                  \
    void IteratorInc(Type##_Iterator * const it)                                                  \
    {                                                                                             \
        return Type##_Iterator_Increment(it);                                                     \
    }                                                                                             \
                                                                                                  \
    void IteratorDec(Type##_Iterator * const it)                                                  \
    {                                                                                             \
        return Type##_Iterator_Decrement(it);                                                     \
    }                                                                                             \
                                                                                                  \
    void IteratorSetValue(Type##_Iterator * const it, MemberType value)                           \
    {                                                                                             \
        Type##_Iterator_SetValue(it, value);                                                      \
    }                                                                                             \
                                                                                                  \
    bool Iterator_Equal(Type##_Iterator * const first, Type##_Iterator * const second)            \
    {                                                                                             \
        return Type##_Iterator_Equal(first, second);                                              \
    }                                                                                             \
                                                                                                  \
    auto Find(Type * const container, MemberType value)                                           \
    {                                                                                             \
        return Type##_Find(container, value);                                                     \
    }                                                                                             \
                                                                                                  \
    auto CustomFind(Type * const container, MemberType value, Type##_compare_t custom_comparator) \
    {                                                                                             \
        return Type##_CustomFind(container, value, custom_comparator);                            \
    }                                                                                             \
                                                                                                  \
    void Clear(Type * const container)                                                            \
    {                                                                                             \
        Type##_Clear(container);                                                                  \
    }

create_wrappers_for_type(VectorTestType, compare_set_ints, int);
create_wrappers_for_type(ListTestType, compare_set_ints, int);
create_wrappers_for_type(DequeTestType, compare_set_ints, int);
create_wrappers_for_type(CustomAllocatorVector, compare_set_ints, int);
create_wrappers_for_type(CustomAllocatorList, compare_set_ints, int);
create_wrappers_for_type(DynamicVector, compare_set_ints, int);
create_wrappers_for_type(DynamicList, compare_set_ints, int);

// Verifies if compiles
create_wrappers_for_type(SVectorWithPointers, compare_set_ints_ptr, int *);
create_wrappers_for_type(StructTypeStaticVector, compare_struct_type, StructType);
create_wrappers_for_type(StructTypeStaticList, compare_struct_type, StructType);
create_wrappers_for_type(StructTypeDynamicVector, compare_struct_type, StructType);
create_wrappers_for_type(StructTypeDynamicList, compare_struct_type, StructType);

create_wrappers_for_type(VectorTestTypeV3, compare_set_ints, int);
create_wrappers_for_type(CustomAllocatorVectorV3, compare_set_ints, int);
create_wrappers_for_type(DynamicVectorV3, compare_set_ints, int);
create_wrappers_for_type(VectorTestTypeV3NullCmp, NULL, int);
create_wrappers_for_type(CustomAllocatorVectorV3NullCmp, NULL, int);
create_wrappers_for_type(DynamicVectorV3NullCmp, NULL, int);

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
struct CompareFuncNotRequiredTest : public testing::Test
{
    T container;
};

using MyTypes = testing::Types<VectorTestType, ListTestType, DequeTestType, CustomAllocatorVector, CustomAllocatorList,
                               DynamicVector, DynamicList, VectorTestTypeV3, CustomAllocatorVectorV3, DynamicVectorV3>;

using StaticContainerTypes = testing::Types<VectorTestType, ListTestType, DequeTestType, VectorTestTypeV3>;

using CustomContainerTypes = testing::Types<CustomAllocatorVector, CustomAllocatorList, CustomAllocatorVectorV3>;

using StructContainerTypes =
    testing::Types<StructTypeStaticVector, StructTypeStaticList, StructTypeDynamicVector, StructTypeDynamicList>;

using CompareFuncNotrequiredTypes =
    testing::Types<VectorTestTypeV3NullCmp, CustomAllocatorVectorV3NullCmp, DynamicVectorV3NullCmp>;

TYPED_TEST_SUITE(ContainerTest, MyTypes);
TYPED_TEST_SUITE(StaticContainerTest, StaticContainerTypes);
TYPED_TEST_SUITE(CustomContainerTest, CustomContainerTypes);
TYPED_TEST_SUITE(StructTypeTest, StructContainerTypes);
TYPED_TEST_SUITE(CompareFuncNotRequiredTest, CompareFuncNotrequiredTypes);

TYPED_TEST(CompareFuncNotRequiredTest, HasNoAssert)
{
    Init(&this->container);
    Deinit(&this->container);
}

TYPED_TEST(CompareFuncNotRequiredTest, AssertIfTryFind)
{
    Init(&this->container);
    EXPECT_DEATH(Find(&this->container, 0), "");
    Deinit(&this->container);
}

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
    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp3);

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
    ASSERT_EQ(IteratorValue(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp1);

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
    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp3);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);

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

    ASSERT_EQ(IteratorValue(&it_begin), temp1);
    ASSERT_EQ(IteratorValue(&it), temp2);
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

    ASSERT_EQ(IteratorValue(&it), temp1);
    IteratorInc(&it);
    ASSERT_EQ(IteratorValue(&it), temp2);
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
    ASSERT_EQ(IteratorValue(&it), temp2);
    IteratorDec(&it);
    ASSERT_EQ(IteratorValue(&it), temp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}

TYPED_TEST(ContainerTest, IteratorAfterInit)
{
    auto it_begin = Begin(&this->container);
    auto it_end   = End(&this->container);

    ASSERT_TRUE(Iterator_Equal(&it_begin, &it_end));
}

TYPED_TEST(ContainerTest, IteratorValues)
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
    IteratorSetValue(&it, newTemp2);
    ASSERT_EQ(IteratorValue(&it), newTemp2);
    IteratorDec(&it);
    uint32_t newTemp1{ 1357 };
    IteratorSetValue(&it, newTemp1);
    ASSERT_EQ(IteratorValue(&it), newTemp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));
}

TYPED_TEST(ContainerTest, IndexValues)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(GetValue(&this->container, 0), temp1);
    ASSERT_EQ(GetValue(&this->container, 1), temp2);

    uint32_t newTemp1{ 1357 };
    uint32_t newTemp2{ 2468 };

    SetValue(&this->container, 0, newTemp1);
    ASSERT_EQ(GetValue(&this->container, 0), newTemp1);
    ASSERT_EQ(GetValue(&this->container, 1), temp2);

    SetValue(&this->container, 1, newTemp2);
    ASSERT_EQ(GetValue(&this->container, 0), newTemp1);
    ASSERT_EQ(GetValue(&this->container, 1), newTemp2);
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

    uint32_t newTemp1{ 1357 };
    uint32_t newTemp2{ 2468 };
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
    ASSERT_EQ(IteratorValue(&it_1), temp1);
    auto it_2 = Find(&this->container, temp2);
    ASSERT_EQ(IteratorValue(&it_2), temp2);
    auto it_3 = Find(&this->container, temp3);
    ASSERT_EQ(IteratorValue(&it_3), temp3);

    auto it_4 = Find(&this->container, temp3 + 1);
    ASSERT_TRUE(Iterator_Equal(&it_4, &end_it));
}

TYPED_TEST(ContainerTest, CustomFinder)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 582 };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);

    auto end_it = End(&this->container);

    auto it_1 = CustomFind(&this->container, temp1 + 1, compare_custom_ints);
    ASSERT_EQ(IteratorValue(&it_1), temp1);
    auto it_2 = CustomFind(&this->container, temp2 + 1, compare_custom_ints);
    ASSERT_EQ(IteratorValue(&it_2), temp2);
    auto it_3 = CustomFind(&this->container, temp3 + 1, compare_custom_ints);
    ASSERT_EQ(IteratorValue(&it_3), temp3);

    auto it_4 = CustomFind(&this->container, temp1, compare_custom_ints);
    ASSERT_TRUE(Iterator_Equal(&it_4, &end_it));
    auto it_5 = CustomFind(&this->container, temp2, compare_custom_ints);
    ASSERT_TRUE(Iterator_Equal(&it_5, &end_it));
    auto it_6 = CustomFind(&this->container, temp3, compare_custom_ints);
    ASSERT_TRUE(Iterator_Equal(&it_6, &end_it));
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
        receivedSet.insert(IteratorValue(&it));
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

        // for(int j = 0; j < 10; j++)
        // {
        //     ASSERT_EQ(*CRef(&this->container, j), j + i + 1);
        // }
    }
}

// TYPED_TEST(ContainerTest, Fifo2)
// {
//     for(int i = 0; i < 10; i++)
//     {
//         auto end = End(&this->container);
//         ASSERT_EQ(Insert(&this->container, i, &end), i + 1);
//     }

//     for(int i = 0; i < 10001; i++)
//     {
//         ASSERT_EQ(Front(&this->container), i);
//         ASSERT_EQ(Back(&this->container), i + 9);

//         auto end = End(&this->container);
//         ASSERT_EQ(Insert(&this->container, i + 10, &end), 11);
//         auto begin = Begin(&this->container);
//         ASSERT_EQ(Erase(&this->container, &begin), 10);

//         for(int j = 0; j < 10; j++)
//         {
//             ASSERT_EQ(*CRef(&this->container, j), j + i + 1);
//         }
//     }
// }

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

    auto receivedVar1 = IteratorValue(&it);
    ASSERT_DOUBLE_EQ(var1.doubleVar, receivedVar1.doubleVar);
    ASSERT_EQ(var1.intVar, receivedVar1.intVar);
    ASSERT_EQ(var1.boolVar, receivedVar1.boolVar);
    ASSERT_EQ(var1.id, receivedVar1.id);

    IteratorInc(&it);
    auto receivedVar2 = IteratorValue(&it);
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

    auto receivedVar2 = IteratorValue(&it);
    ASSERT_DOUBLE_EQ(var2.doubleVar, receivedVar2.doubleVar);
    ASSERT_EQ(var2.intVar, receivedVar2.intVar);
    ASSERT_EQ(var2.boolVar, receivedVar2.boolVar);
    ASSERT_EQ(var2.id, receivedVar2.id);

    IteratorInc(&it);
    auto receivedVar1 = IteratorValue(&it);
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

    auto receivedVar2 = IteratorValue(&it);
    ASSERT_DOUBLE_EQ(var2.doubleVar, receivedVar2.doubleVar);
    ASSERT_EQ(var2.intVar, receivedVar2.intVar);
    ASSERT_EQ(var2.boolVar, receivedVar2.boolVar);
    ASSERT_EQ(var2.id, receivedVar2.id);

    IteratorInc(&it);
    auto receivedVar1 = IteratorValue(&it);
    ASSERT_DOUBLE_EQ(var1.doubleVar, receivedVar1.doubleVar);
    ASSERT_EQ(var1.intVar, receivedVar1.intVar);
    ASSERT_EQ(var1.boolVar, receivedVar1.boolVar);
    ASSERT_EQ(var1.id, receivedVar1.id);
}

// must to at the end to verify if we have valid declarations
static_vector_impl(VectorTestTypeV3, int, CONTAINER_CAPACITY);
custom_allocator_vector_impl(CustomAllocatorVectorV3, int, DynamicAllocator);
dynamic_vector_impl(DynamicVectorV3, int);
static_vector_impl(VectorTestTypeV3NullCmp, int, CONTAINER_CAPACITY);
custom_allocator_vector_impl(CustomAllocatorVectorV3NullCmp, int, DynamicAllocator);
dynamic_vector_impl(DynamicVectorV3NullCmp, int);