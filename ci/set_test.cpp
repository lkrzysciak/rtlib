#include "gtest/gtest.h"
#include "rtlib/memory.h"
#include "rtlib/set.h"
#include "rtlib/unordered_set.h"
#include "rtlib/comparator.h"
#include "rtlib/hash.h"
#include <map>
#include <set>
#include <list>
#include <numeric>
#include <algorithm>
#include <array>
#include <vector>

typedef struct
{
    double doubleVar;
    int intVar;
    bool boolVar;
    uint64_t id;
} __attribute__((packed)) StructType;  // use packed to avoid any padding issues (e.g. with random hashes)

typedef int * IntPtr;

private_hash(int);
private_hash_impl(int);

comparator(int);

private_hash(IntPtr);
private_hash_impl(IntPtr);

comparator(IntPtr);

private_hash(StructType);
private_hash_impl(StructType);

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

#define CONTAINER_CAPACITY 100
#define SMALL_CONTAINER_CAPACITY 4

dynamic_memory(MyDynamicAllocator);
dynamic_memory_impl(MyDynamicAllocator);

static_set(SSetWithInt, int, CONTAINER_CAPACITY);
custom_allocator_set(CSetWithInt, int, MyDynamicAllocator);
dynamic_set(DSetWithInt, int);
static_set(SSetWithPointer, IntPtr, CONTAINER_CAPACITY);
custom_allocator_set(CSetWithPointer, IntPtr, MyDynamicAllocator);
dynamic_set(DSetWithPointer, IntPtr);
static_set(SSetWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_set(CSetWithStruct, StructType, MyDynamicAllocator);
dynamic_set(DSetWithStruct, StructType);

static_unordered_set(SUnorderedSetWithInt, int, CONTAINER_CAPACITY);
custom_allocator_unordered_set(CUnorderedSetWithInt, int, MyDynamicAllocator);
dynamic_unordered_set(DUnorderedSetWithInt, int);
static_unordered_set(SUnorderedSetWithPointer, IntPtr, CONTAINER_CAPACITY);
custom_allocator_unordered_set(CUnorderedSetWithPointer, IntPtr, MyDynamicAllocator);
dynamic_unordered_set(DUnorderedSetWithPointer, IntPtr);
static_unordered_set(SUnorderedSetWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_unordered_set(CUnorderedSetWithStruct, StructType, MyDynamicAllocator);
dynamic_unordered_set(DUnorderedSetWithStruct, StructType);

static_unordered_set(SmallStaticUnorderedSet, int, SMALL_CONTAINER_CAPACITY);

#define create_wrappers_for_type(Type, MemberType)                                     \
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
    int Insert(Type * const container, MemberType value)                               \
    {                                                                                  \
        return Type##_Insert(container, value);                                        \
    }                                                                                  \
                                                                                       \
    int Erase(Type * const container, Type##_Iterator * it)                            \
    {                                                                                  \
        return Type##_Erase(container, it);                                            \
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
    auto * CRef(Type##_Iterator * const it)                                            \
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

create_wrappers_for_type(SSetWithInt, int);
create_wrappers_for_type(CSetWithInt, int);
create_wrappers_for_type(DSetWithInt, int);
create_wrappers_for_type(SSetWithPointer, IntPtr);
create_wrappers_for_type(CSetWithPointer, IntPtr);
create_wrappers_for_type(DSetWithPointer, IntPtr);
create_wrappers_for_type(SSetWithStruct, StructType);
create_wrappers_for_type(CSetWithStruct, StructType);
create_wrappers_for_type(DSetWithStruct, StructType);
create_wrappers_for_type(SUnorderedSetWithInt, int);
create_wrappers_for_type(CUnorderedSetWithInt, int);
create_wrappers_for_type(DUnorderedSetWithInt, int);
create_wrappers_for_type(SUnorderedSetWithPointer, IntPtr);
create_wrappers_for_type(CUnorderedSetWithPointer, IntPtr);
create_wrappers_for_type(DUnorderedSetWithPointer, IntPtr);
create_wrappers_for_type(SUnorderedSetWithStruct, StructType);
create_wrappers_for_type(CUnorderedSetWithStruct, StructType);
create_wrappers_for_type(DUnorderedSetWithStruct, StructType);

template<typename T>
struct SetTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct StaticSetTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct OrderedSetTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct SetStructTypeTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct SetPointerTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

using MyTypes = testing::Types<SSetWithInt, CSetWithInt, DSetWithInt, SUnorderedSetWithInt, CUnorderedSetWithInt,
                               DUnorderedSetWithInt>;

using StaticContainerTypes = testing::Types<SSetWithInt, SUnorderedSetWithInt>;

using OrderedSetTypes = testing::Types<SSetWithInt, CSetWithInt, DSetWithInt>;

using StructContainerTypes = testing::Types<SSetWithStruct, CSetWithStruct, DSetWithStruct, SUnorderedSetWithStruct,
                                            CUnorderedSetWithStruct, DUnorderedSetWithStruct>;

using TypesWithPointer = testing::Types<SSetWithPointer, CSetWithPointer, DSetWithPointer, SUnorderedSetWithPointer,
                                        CUnorderedSetWithPointer, DUnorderedSetWithPointer>;

TYPED_TEST_CASE(SetTest, MyTypes);
TYPED_TEST_CASE(StaticSetTest, StaticContainerTypes);
TYPED_TEST_CASE(SetStructTypeTest, StructContainerTypes);
TYPED_TEST_CASE(SetPointerTest, TypesWithPointer);
TYPED_TEST_CASE(OrderedSetTest, OrderedSetTypes);

TYPED_TEST(SetTest, IsEmptyAfterInit)
{
    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(SetTest, InsertVerifyFromBeginToEnd)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    std::set<int> expected_set{ temp1, temp2, temp3, temp4, temp5 };

    ASSERT_EQ(Insert(&this->container, temp1), 1);
    ASSERT_EQ(Insert(&this->container, temp2), 2);
    ASSERT_EQ(Insert(&this->container, temp3), 3);
    ASSERT_EQ(Insert(&this->container, temp4), 4);
    ASSERT_EQ(Insert(&this->container, temp5), 5);

    std::set<int> to_compare_set{};

    auto it  = Begin(&this->container);
    auto end = End(&this->container);

    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set, to_compare_set);
    ASSERT_EQ(Size(&this->container), 5);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(SetTest, InsertVerifyFromEndToBegin)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    std::set<int> expected_set{ temp1, temp2, temp3, temp4, temp5 };

    ASSERT_EQ(Insert(&this->container, temp1), 1);
    ASSERT_EQ(Insert(&this->container, temp2), 2);
    ASSERT_EQ(Insert(&this->container, temp3), 3);
    ASSERT_EQ(Insert(&this->container, temp4), 4);
    ASSERT_EQ(Insert(&this->container, temp5), 5);

    std::set<int> to_compare_set{};

    auto begin = Begin(&this->container);
    auto it    = End(&this->container);

    while(!Iterator_Equal(&it, &begin))
    {
        IteratorDec(&it);
        to_compare_set.insert(*CRef(&it));
    }
    ASSERT_EQ(expected_set, to_compare_set);
    ASSERT_EQ(Size(&this->container), 5);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(SetTest, EraseVerifyFromBeginToEnd)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1);
    Insert(&this->container, temp2);
    Insert(&this->container, temp3);
    Insert(&this->container, temp4);
    Insert(&this->container, temp5);

    std::set<int> to_compare_set{};

    auto it  = Begin(&this->container);
    auto end = End(&this->container);

    /* Remove temp4 ***************************************************/
    // Get temp4 iterator
    while(*CRef(&it) != temp4)
    {
        IteratorInc(&it);
    }

    // Remove temp4
    ASSERT_EQ(Erase(&this->container, &it), 4);

    std::set<int> expected_set_1{ temp1, temp2, temp3, temp5 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_1, to_compare_set);
    ASSERT_EQ(Size(&this->container), 4);

    /* Remove temp2 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it) != temp2)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&this->container, &it), 3);

    std::set<int> expected_set_2{ temp1, temp3, temp5 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_2, to_compare_set);
    ASSERT_EQ(Size(&this->container), 3);

    /* Remove temp1 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it) != temp1)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&this->container, &it), 2);

    std::set<int> expected_set_3{ temp3, temp5 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_3, to_compare_set);
    ASSERT_EQ(Size(&this->container), 2);

    /* Remove temp5 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it) != temp5)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&this->container, &it), 1);

    std::set<int> expected_set_4{ temp3 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_4, to_compare_set);
    ASSERT_EQ(Size(&this->container), 1);

    /* Remove temp3 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it) != temp3)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&this->container, &it), 0);

    std::set<int> expected_set_5{};

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_5, to_compare_set);
    ASSERT_EQ(Size(&this->container), 0);

    /* Insert temp6 ****************************************/
    int temp6{ 4321 };
    ASSERT_EQ(Insert(&this->container, temp6), 1);
    std::set<int> expected_set_6{ temp6 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_6, to_compare_set);
    ASSERT_EQ(Size(&this->container), 1);
}

TYPED_TEST(OrderedSetTest, IterationMatchesSortedOrderAcrossPermutations)
{
    std::array<int, 5> values{ { 1, 2, 3, 4, 5 } };
    std::array<int, 5> sorted = values;

    do
    {
        Clear(&this->container);
        for(const auto value : values)
        {
            Insert(&this->container, value);
        }

        std::vector<int> collected;
        collected.reserve(values.size());

        auto it  = Begin(&this->container);
        auto end = End(&this->container);

        size_t guard = 0;
        while(!Iterator_Equal(&it, &end) && guard <= values.size())
        {
            collected.push_back(*CRef(&it));
            IteratorInc(&it);
            ++guard;
        }

        ASSERT_EQ(collected.size(), values.size());
        ASSERT_EQ(collected, std::vector<int>(sorted.begin(), sorted.end()));
    } while(std::next_permutation(values.begin(), values.end()));
}

TEST(StaticUnorderedSetTest, IterationDoesNotFollowStaleNextPointers)
{
    SmallStaticUnorderedSet container;
    SmallStaticUnorderedSet_Construct(&container);

    int key1         = 0;
    int key2         = 0;
    int key5         = 0;
    bool found_two   = false;
    bool found_three = false;

    for(int a = 1; a < 500 && !found_two; ++a)
    {
        for(int b = a + 1; b < 500 && !found_two; ++b)
        {
            if(int_Hash(&a) % SMALL_CONTAINER_CAPACITY == int_Hash(&b) % SMALL_CONTAINER_CAPACITY)
            {
                key1      = a;
                key2      = b;
                found_two = true;
            }
        }
    }

    for(int c = 1; c < 500 && !found_three; ++c)
    {
        if(c != key1 && c != key2 &&
           int_Hash(&c) % SMALL_CONTAINER_CAPACITY == int_Hash(&key1) % SMALL_CONTAINER_CAPACITY)
        {
            key5        = c;
            found_three = true;
        }
    }

    ASSERT_TRUE(found_two);
    ASSERT_TRUE(found_three);

    ASSERT_EQ(SmallStaticUnorderedSet_Insert(&container, key1), 1);
    ASSERT_EQ(SmallStaticUnorderedSet_Insert(&container, key2), 2);

    auto it_erase = SmallStaticUnorderedSet_Find(&container, key1);
    ASSERT_EQ(SmallStaticUnorderedSet_Erase(&container, &it_erase), 1);

    ASSERT_EQ(SmallStaticUnorderedSet_Insert(&container, 101), 2);
    ASSERT_EQ(SmallStaticUnorderedSet_Insert(&container, 202), 3);
    ASSERT_EQ(SmallStaticUnorderedSet_Insert(&container, key5), 4);

    std::set<int> collected;
    auto it  = SmallStaticUnorderedSet_Begin(&container);
    auto end = SmallStaticUnorderedSet_End(&container);

    size_t guard = 0;
    while(!SmallStaticUnorderedSet_Iterator_Equal(&it, &end) && guard <= SMALL_CONTAINER_CAPACITY + 2)
    {
        collected.insert(*SmallStaticUnorderedSet_Iterator_CRef(&it));
        SmallStaticUnorderedSet_Iterator_Increment(&it);
        ++guard;
    }

    ASSERT_EQ(collected.size(), 4u);
    ASSERT_TRUE(collected.find(key2) != collected.end());
    ASSERT_TRUE(collected.find(key5) != collected.end());
    ASSERT_TRUE(collected.find(101) != collected.end());
    ASSERT_TRUE(collected.find(202) != collected.end());
    ASSERT_TRUE(SmallStaticUnorderedSet_Iterator_Equal(&it, &end));

    SmallStaticUnorderedSet_Destruct(&container);
}

TYPED_TEST(SetTest, FindExistedMember)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1);
    Insert(&this->container, temp2);
    Insert(&this->container, temp3);
    Insert(&this->container, temp4);
    Insert(&this->container, temp5);

    auto temp1It = Find(&this->container, temp1);
    ASSERT_EQ(*CRef(&temp1It), temp1);

    auto temp2It = Find(&this->container, temp2);
    ASSERT_EQ(*CRef(&temp2It), temp2);

    auto temp3It = Find(&this->container, temp3);
    ASSERT_EQ(*CRef(&temp3It), temp3);

    auto temp4It = Find(&this->container, temp4);
    ASSERT_EQ(*CRef(&temp4It), temp4);

    auto temp5It = Find(&this->container, temp5);
    ASSERT_EQ(*CRef(&temp5It), temp5);
}

TYPED_TEST(SetTest, CRef)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1);
    Insert(&this->container, temp2);
    Insert(&this->container, temp3);
    Insert(&this->container, temp4);
    Insert(&this->container, temp5);

    auto temp1It = Find(&this->container, temp1);
    ASSERT_EQ(*CRef(&temp1It), temp1);

    auto temp2It = Find(&this->container, temp2);
    ASSERT_EQ(*CRef(&temp2It), temp2);

    auto temp3It = Find(&this->container, temp3);
    ASSERT_EQ(*CRef(&temp3It), temp3);

    auto temp4It = Find(&this->container, temp4);
    ASSERT_EQ(*CRef(&temp4It), temp4);

    auto temp5It = Find(&this->container, temp5);
    ASSERT_EQ(*CRef(&temp5It), temp5);
}

TYPED_TEST(SetTest, FindNonExistedMember)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };
    int temp6{ 753 };

    Insert(&this->container, temp1);
    Insert(&this->container, temp2);
    Insert(&this->container, temp3);
    Insert(&this->container, temp4);
    Insert(&this->container, temp5);

    auto end = End(&this->container);

    auto temp6It = Find(&this->container, temp6);
    ASSERT_TRUE(Iterator_Equal(&temp6It, &end));
}

TYPED_TEST(SetTest, IncrementAndDecrementIterator)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1);
    Insert(&this->container, temp2);
    Insert(&this->container, temp3);
    Insert(&this->container, temp4);
    Insert(&this->container, temp5);

    auto it = Begin(&this->container);

    IteratorInc(&it);
    IteratorInc(&it);
    IteratorInc(&it);

    auto it1Value = *CRef(&it);

    IteratorInc(&it);
    IteratorDec(&it);
    auto it1_1Value = *CRef(&it);
    ASSERT_EQ(it1Value, it1_1Value);

    IteratorDec(&it);
    IteratorInc(&it);
    auto it1_2Value = *CRef(&it);
    ASSERT_EQ(it1Value, it1_2Value);
}

TYPED_TEST(SetTest, AddedExistingElement)
{
    int temp1{ 3215 };

    ASSERT_EQ(Insert(&this->container, temp1), 1);
    ASSERT_EQ(Insert(&this->container, temp1), ELEMENT_EXISTS);

    ASSERT_EQ(Size(&this->container), 1);
}

TYPED_TEST(SetTest, Permutations)
{
    std::vector<int> testPermutation{
        10,   20,  50,  1,   158, 78,  254, -8,  8756, 51,  4,    5,    1024, 85,    697,
        4587, 123, 258, 741, 963, 951, 843, 628, 762,  384, 6969, 5454, 8514, 74569, 8546
    };
    for(size_t idx = 0; idx < testPermutation.size(); ++idx)
    {
        ASSERT_EQ((idx + 1), Insert(&this->container, testPermutation[idx]));
    }

    std::set<int> expectedSet{ testPermutation.begin(), testPermutation.end() };
    std::set<int> receivedSet{};
    auto endIt = End(&this->container);
    for(auto it = Begin(&this->container); !Iterator_Equal(&it, &endIt); IteratorInc(&it))
    {
        auto val = *CRef(&it);
        receivedSet.insert(val);
    }
    ASSERT_EQ(expectedSet, receivedSet);
}

TYPED_TEST(SetTest, AddAndEraseMultipleTimes)
{
    for(int idx = 0; idx < 100; idx++)
    {
        ASSERT_EQ((idx + 1), Insert(&this->container, idx));
    }

    // delete first 25 records
    for(int idx = 0; idx < 25; idx++)
    {
        auto it = Find(&this->container, idx);
        ASSERT_EQ((100 - idx - 1), Erase(&this->container, &it));
    }

    // delete last 25 records
    for(int idx = 0; idx < 25; idx++)
    {
        auto it = Find(&this->container, idx + 75);
        ASSERT_EQ((75 - idx - 1), Erase(&this->container, &it));
    }

    for(int idx = 0; idx < 25; idx++)
    {
        auto it = Find(&this->container, 2 * idx + 25);
        ASSERT_EQ((50 - idx - 1), Erase(&this->container, &it));
    }

    std::set<int> expectedSet{ 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50,
                               52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74 };
    std::set<int> receivedSet{};
    auto endIt = End(&this->container);
    for(auto it = Begin(&this->container); !Iterator_Equal(&it, &endIt); IteratorInc(&it))
    {
        auto val = *CRef(&it);
        receivedSet.insert(val);
    }
    ASSERT_EQ(expectedSet, receivedSet);
}

TYPED_TEST(SetTest, Clear)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    Insert(&this->container, temp1);
    Insert(&this->container, temp2);
    Insert(&this->container, temp3);
    ASSERT_EQ(Size(&this->container), 3);

    Clear(&this->container);

    ASSERT_EQ(Size(&this->container), 0);
}

TYPED_TEST(StaticSetTest, InsertOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(Insert(&this->container, temp1 + i), i + 1);
    }
    ASSERT_EQ(Insert(&this->container, temp1), ALLOCATION_ERROR);

    auto it  = Begin(&this->container);
    auto end = End(&this->container);
    int i    = 0;
    for(; !Iterator_Equal(&it, &end); IteratorInc(&it))
    {
        i++;
    }
    ASSERT_EQ(i, CONTAINER_CAPACITY);
}

TYPED_TEST(StaticSetTest, InsertOverLimitExistingElements)
{
    uint32_t temp1{ 3215 };

    ASSERT_EQ(Insert(&this->container, temp1), 1);

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(Insert(&this->container, temp1), ELEMENT_EXISTS);
    }
    ASSERT_EQ(Size(&this->container), 1);
}

TYPED_TEST(SetStructTypeTest, StructMembersInsert)
{
    StructType var1 = { .doubleVar = 159.753, .intVar = 0x12345678, .boolVar = true, .id = 0x1 };
    StructType var2 = { .doubleVar = 0.569, .intVar = 0x1, .boolVar = false, .id = 0xffffffffffffffff };

    ASSERT_EQ(Insert(&this->container, var1), 1);
    ASSERT_EQ(Insert(&this->container, var2), 2);

    // Ordered sets iterate in key order; unordered sets do not guarantee any iteration order.
    // Validate that both inserted elements are present and intact, regardless of iteration order.
    auto it        = Begin(&this->container);
    auto receivedA = *CRef(&it);
    IteratorInc(&it);
    auto receivedB = *CRef(&it);

    auto expect_struct_eq = [](const StructType & expected, const StructType & received) {
        EXPECT_DOUBLE_EQ(expected.doubleVar, received.doubleVar);
        EXPECT_EQ(expected.intVar, received.intVar);
        EXPECT_EQ(expected.boolVar, received.boolVar);
        EXPECT_EQ(expected.id, received.id);
    };

    if(receivedA.id == var1.id)
    {
        expect_struct_eq(var1, receivedA);
        expect_struct_eq(var2, receivedB);
    }
    else if(receivedA.id == var2.id)
    {
        expect_struct_eq(var2, receivedA);
        expect_struct_eq(var1, receivedB);
    }
    else
    {
        FAIL() << "Unexpected element id in container iteration";
    }
}

TYPED_TEST(SetPointerTest, Insert)
{
    int * a = new int(5);
    int * b = new int(11);
    std::set<int *> expected{ a, b };

    ASSERT_EQ(Insert(&this->container, a), 1);
    ASSERT_EQ(Insert(&this->container, b), 2);

    std::set<int *> received{};
    auto it = Begin(&this->container);
    received.insert(*CRef(&it));
    IteratorInc(&it);
    received.insert(*CRef(&it));

    ASSERT_EQ(expected, received);

    Clear(&this->container);

    delete a;
    delete b;
}

static_set_impl(SSetWithInt, int, CONTAINER_CAPACITY);
custom_allocator_set_impl(CSetWithInt, int, MyDynamicAllocator);
dynamic_set_impl(DSetWithInt, int);
static_set_impl(SSetWithPointer, IntPtr, CONTAINER_CAPACITY);
custom_allocator_set_impl(CSetWithPointer, IntPtr, MyDynamicAllocator);
dynamic_set_impl(DSetWithPointer, IntPtr);
static_set_impl(SSetWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_set_impl(CSetWithStruct, StructType, MyDynamicAllocator);
dynamic_set_impl(DSetWithStruct, StructType);

static_unordered_set_impl(SUnorderedSetWithInt, int, CONTAINER_CAPACITY);
custom_allocator_unordered_set_impl(CUnorderedSetWithInt, int, MyDynamicAllocator);
dynamic_unordered_set_impl(DUnorderedSetWithInt, int);
static_unordered_set_impl(SUnorderedSetWithPointer, IntPtr, CONTAINER_CAPACITY);
custom_allocator_unordered_set_impl(CUnorderedSetWithPointer, IntPtr, MyDynamicAllocator);
dynamic_unordered_set_impl(DUnorderedSetWithPointer, IntPtr);
static_unordered_set_impl(SUnorderedSetWithStruct, StructType, CONTAINER_CAPACITY);
custom_allocator_unordered_set_impl(CUnorderedSetWithStruct, StructType, MyDynamicAllocator);
dynamic_unordered_set_impl(DUnorderedSetWithStruct, StructType);
static_unordered_set_impl(SmallStaticUnorderedSet, int, SMALL_CONTAINER_CAPACITY);