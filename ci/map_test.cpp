#include "gtest/gtest.h"
#include "rtlib/map.h"
#include "rtlib/unordered_map.h"
#include <map>
#include <set>
#include <list>
#include <numeric>

#define CONTAINER_CAPACITY 100

static_map(StaticMapV3, int, int, CONTAINER_CAPACITY);
dynamic_map(DynamicMapV3, int, int);
static_unordered_map(StaticUnorderedMapV3, int, int, CONTAINER_CAPACITY);
dynamic_unordered_map(DynamicUnorderedMapV3, int, int);

static unsigned int int_Hash(const int * value)
{
    return *value;
}

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

#define create_wrappers_for_type(Type, Key, Value)                                     \
    void Init(Type * const container)                                                  \
    {                                                                                  \
        Type##_Construct(container);                                                   \
    }                                                                                  \
                                                                                       \
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
    int Insert(Type * const container, Key key, Value value)                           \
    {                                                                                  \
        return Type##_Insert(container, key, value);                                   \
    }                                                                                  \
                                                                                       \
    int Erase(Type * const container, Type##_Iterator * it)                            \
    {                                                                                  \
        return Type##_Erase(container, it);                                            \
    }                                                                                  \
    auto Ref(Type * const container, Key key)                                          \
    {                                                                                  \
        return Type##_Ref(container, key);                                             \
    }                                                                                  \
                                                                                       \
    auto CRef(Type * const container, Key key)                                         \
    {                                                                                  \
        return Type##_CRef(container, key);                                            \
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
    auto Ref(Type##_Iterator * const it)                                               \
    {                                                                                  \
        return Type##_Iterator_Ref(it);                                                \
    }                                                                                  \
                                                                                       \
    auto CRef(Type##_Iterator * const it)                                              \
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
    auto Find(Type * const container, Key value)                                       \
    {                                                                                  \
        return Type##_Find(container, value);                                          \
    }                                                                                  \
                                                                                       \
    void Clear(Type * const container)                                                 \
    {                                                                                  \
        Type##_Clear(container);                                                       \
    }

create_wrappers_for_type(StaticMapV3, int, int);
create_wrappers_for_type(DynamicMapV3, int, int);
create_wrappers_for_type(StaticUnorderedMapV3, int, int);
create_wrappers_for_type(DynamicUnorderedMapV3, int, int);

template<typename T>
struct MapTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct StaticMapTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

using AllTypes = testing::Types<StaticMapV3, DynamicMapV3, StaticUnorderedMapV3, DynamicUnorderedMapV3>;

using StaticTypes = testing::Types<StaticMapV3, StaticUnorderedMapV3>;

TYPED_TEST_CASE(MapTest, AllTypes);
TYPED_TEST_CASE(StaticMapTest, StaticTypes);

TYPED_TEST(MapTest, IsEmptyAfterInit)
{
    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
}

TYPED_TEST(MapTest, Ref)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    ASSERT_EQ(Insert(&this->container, temp1, 0), 1);
    ASSERT_EQ(Insert(&this->container, temp2, 1), 2);
    ASSERT_EQ(Insert(&this->container, temp3, 2), 3);
    ASSERT_EQ(Insert(&this->container, temp4, 3), 4);
    ASSERT_EQ(Insert(&this->container, temp5, 4), 5);

    ASSERT_EQ(*Ref(&this->container, temp1), 0);
    ASSERT_EQ(*Ref(&this->container, temp2), 1);
    ASSERT_EQ(*Ref(&this->container, temp3), 2);
    ASSERT_EQ(*Ref(&this->container, temp4), 3);
    ASSERT_EQ(*Ref(&this->container, temp5), 4);

    *Ref(&this->container, temp1) = 10;
    *Ref(&this->container, temp2) = 11;
    *Ref(&this->container, temp3) = 12;
    *Ref(&this->container, temp4) = 13;
    *Ref(&this->container, temp5) = 14;

    ASSERT_EQ(*Ref(&this->container, temp1), 10);
    ASSERT_EQ(*Ref(&this->container, temp2), 11);
    ASSERT_EQ(*Ref(&this->container, temp3), 12);
    ASSERT_EQ(*Ref(&this->container, temp4), 13);
    ASSERT_EQ(*Ref(&this->container, temp5), 14);
}

TYPED_TEST(MapTest, CRef)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    ASSERT_EQ(Insert(&this->container, temp1, 0), 1);
    ASSERT_EQ(Insert(&this->container, temp2, 1), 2);
    ASSERT_EQ(Insert(&this->container, temp3, 2), 3);
    ASSERT_EQ(Insert(&this->container, temp4, 3), 4);
    ASSERT_EQ(Insert(&this->container, temp5, 4), 5);

    ASSERT_EQ(*CRef(&this->container, temp1), 0);
    ASSERT_EQ(*CRef(&this->container, temp2), 1);
    ASSERT_EQ(*CRef(&this->container, temp3), 2);
    ASSERT_EQ(*CRef(&this->container, temp4), 3);
    ASSERT_EQ(*CRef(&this->container, temp5), 4);
}

TYPED_TEST(MapTest, InsertVerifyFromBeginToEnd)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    std::set<int> expected_set{ temp1, temp2, temp3, temp4, temp5 };

    ASSERT_EQ(Insert(&this->container, temp1, 0), 1);
    ASSERT_EQ(Insert(&this->container, temp2, 1), 2);
    ASSERT_EQ(Insert(&this->container, temp3, 2), 3);
    ASSERT_EQ(Insert(&this->container, temp4, 3), 4);
    ASSERT_EQ(Insert(&this->container, temp5, 4), 5);

    std::set<int> to_compare_set{};

    auto it  = Begin(&this->container);
    auto end = End(&this->container);

    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it).first);
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set, to_compare_set);
    ASSERT_EQ(Size(&this->container), 5);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(MapTest, InsertVerifyFromEndToBegin)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    std::set<int> expected_set{ temp1, temp2, temp3, temp4, temp5 };

    ASSERT_EQ(Insert(&this->container, temp1, 0), 1);
    ASSERT_EQ(Insert(&this->container, temp2, 1), 2);
    ASSERT_EQ(Insert(&this->container, temp3, 2), 3);
    ASSERT_EQ(Insert(&this->container, temp4, 3), 4);
    ASSERT_EQ(Insert(&this->container, temp5, 4), 5);

    std::set<int> to_compare_set{};

    auto begin = Begin(&this->container);
    auto it    = End(&this->container);

    while(!Iterator_Equal(&it, &begin))
    {
        IteratorDec(&it);
        to_compare_set.insert(*CRef(&it).first);
    }
    ASSERT_EQ(expected_set, to_compare_set);
    ASSERT_EQ(Size(&this->container), 5);
    ASSERT_FALSE(Empty(&this->container));
}

TYPED_TEST(MapTest, EraseVerifyFromBeginToEnd)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1, 0);
    Insert(&this->container, temp2, 1);
    Insert(&this->container, temp3, 2);
    Insert(&this->container, temp4, 3);
    Insert(&this->container, temp5, 4);

    std::set<int> to_compare_set{};

    auto it  = Begin(&this->container);
    auto end = End(&this->container);

    /* Remove temp4 ***************************************************/
    // Get temp4 iterator
    while(*CRef(&it).first != temp4)
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
        to_compare_set.insert(*CRef(&it).first);
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_1, to_compare_set);
    ASSERT_EQ(Size(&this->container), 4);

    /* Remove temp2 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it).first != temp2)
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
        to_compare_set.insert(*CRef(&it).first);
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_2, to_compare_set);
    ASSERT_EQ(Size(&this->container), 3);

    /* Remove temp1 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it).first != temp1)
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
        to_compare_set.insert(*CRef(&it).first);
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_3, to_compare_set);
    ASSERT_EQ(Size(&this->container), 2);

    /* Remove temp5 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it).first != temp5)
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
        to_compare_set.insert(*CRef(&it).first);
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_4, to_compare_set);
    ASSERT_EQ(Size(&this->container), 1);

    /* Remove temp3 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(*CRef(&it).first != temp3)
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
        to_compare_set.insert(*CRef(&it).first);
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_5, to_compare_set);
    ASSERT_EQ(Size(&this->container), 0);

    /* Insert temp6 ****************************************/
    int temp6{ 4321 };
    ASSERT_EQ(Insert(&this->container, temp6, 5), 1);
    std::set<int> expected_set_6{ temp6 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(*CRef(&it).first);
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_6, to_compare_set);
    ASSERT_EQ(Size(&this->container), 1);
}

TYPED_TEST(MapTest, ModifyContainerValues)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1, 0);
    Insert(&this->container, temp2, 1);
    Insert(&this->container, temp3, 2);
    Insert(&this->container, temp4, 3);
    Insert(&this->container, temp5, 4);

    std::map<int, int> to_compare_set{};
    auto it  = Begin(&this->container);
    auto end = End(&this->container);

    /* Change temp4 value */
    while(*CRef(&it).first != temp4)
    {
        IteratorInc(&it);
    }

    *Ref(&it).second = 333;

    std::map<int, int> expected_set_1{ { temp1, 0 }, { temp2, 1 }, { temp3, 2 }, { temp4, 333 }, { temp5, 4 } };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set[*CRef(&it).first] = *CRef(&it).second;
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_1, to_compare_set);

    /* Change temp1 value */
    it  = Begin(&this->container);
    end = End(&this->container);
    while(*CRef(&it).first != temp1)
    {
        IteratorInc(&it);
    }

    *Ref(&it).second = 6;

    std::map<int, int> expected_set_2{ { temp1, 6 }, { temp2, 1 }, { temp3, 2 }, { temp4, 333 }, { temp5, 4 } };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set[*CRef(&it).first] = *CRef(&it).second;
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_2, to_compare_set);
}

TYPED_TEST(MapTest, FindExistedMember)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1, 0);
    Insert(&this->container, temp2, 1);
    Insert(&this->container, temp3, 2);
    Insert(&this->container, temp4, 3);
    Insert(&this->container, temp5, 4);

    auto temp1It = Find(&this->container, temp1);
    ASSERT_EQ(*CRef(&temp1It).first, temp1);
    ASSERT_EQ(*CRef(&temp1It).second, 0);

    auto temp2It = Find(&this->container, temp2);
    ASSERT_EQ(*CRef(&temp2It).first, temp2);
    ASSERT_EQ(*CRef(&temp2It).second, 1);

    auto temp3It = Find(&this->container, temp3);
    ASSERT_EQ(*CRef(&temp3It).first, temp3);
    ASSERT_EQ(*CRef(&temp3It).second, 2);

    auto temp4It = Find(&this->container, temp4);
    ASSERT_EQ(*CRef(&temp4It).first, temp4);
    ASSERT_EQ(*CRef(&temp4It).second, 3);

    auto temp5It = Find(&this->container, temp5);
    ASSERT_EQ(*CRef(&temp5It).first, temp5);
    ASSERT_EQ(*CRef(&temp5It).second, 4);
}

TYPED_TEST(MapTest, FindNonExistedMember)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };
    int temp6{ 753 };

    Insert(&this->container, temp1, 0);
    Insert(&this->container, temp2, 1);
    Insert(&this->container, temp3, 2);
    Insert(&this->container, temp4, 3);
    Insert(&this->container, temp5, 4);

    auto end = End(&this->container);

    auto temp6It = Find(&this->container, temp6);
    ASSERT_TRUE(Iterator_Equal(&temp6It, &end));
}

TYPED_TEST(MapTest, IncrementAndDecrementIterator)
{
    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&this->container, temp1, 0);
    Insert(&this->container, temp2, 1);
    Insert(&this->container, temp3, 2);
    Insert(&this->container, temp4, 3);
    Insert(&this->container, temp5, 4);

    auto it = Begin(&this->container);

    IteratorInc(&it);
    IteratorInc(&it);
    IteratorInc(&it);

    auto it1Value = CRef(&it);

    IteratorInc(&it);
    IteratorDec(&it);
    auto it1_1Value = CRef(&it);
    ASSERT_EQ(it1Value.first, it1_1Value.first);
    ASSERT_EQ(it1Value.second, it1_1Value.second);

    IteratorDec(&it);
    IteratorInc(&it);
    auto it1_2Value = CRef(&it);
    ASSERT_EQ(it1Value.first, it1_2Value.first);
    ASSERT_EQ(it1Value.second, it1_2Value.second);
}

TYPED_TEST(MapTest, AddedExistingElement)
{
    int temp1{ 3215 };

    ASSERT_EQ(Insert(&this->container, temp1, 0), 1);
    ASSERT_EQ(Insert(&this->container, temp1, 0), ELEMENT_EXISTS);

    ASSERT_EQ(Size(&this->container), 1);
}

TYPED_TEST(MapTest, Permutations)
{
    std::vector<int> testPermutation{
        10,   20,  50,  1,   158, 78,  254, -8,  8756, 51,  4,    5,    1024, 85,    697,
        4587, 123, 258, 741, 963, 951, 843, 628, 762,  384, 6969, 5454, 8514, 74569, 8546
    };

    for(size_t idx = 0; idx < testPermutation.size(); ++idx)
    {
        ASSERT_EQ((idx + 1), Insert(&this->container, testPermutation[idx], idx));
    }

    std::set<int> expectedSet{ testPermutation.begin(), testPermutation.end() };
    std::set<int> receivedSet{};
    auto endIt = End(&this->container);
    for(auto it = Begin(&this->container); !Iterator_Equal(&it, &endIt); IteratorInc(&it))
    {
        receivedSet.insert(*CRef(&it).first);
    }
    ASSERT_EQ(expectedSet, receivedSet);
}

TYPED_TEST(MapTest, Clear)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 980 };

    Insert(&this->container, temp1, 0);
    Insert(&this->container, temp2, 1);
    Insert(&this->container, temp3, 2);
    ASSERT_EQ(Size(&this->container), 3);

    Clear(&this->container);

    ASSERT_EQ(Size(&this->container), 0);
}

TYPED_TEST(StaticMapTest, InsertOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(Insert(&this->container, temp1 + i, i), i + 1);
    }
    ASSERT_EQ(Insert(&this->container, temp1, 0), ALLOCATION_ERROR);
}

TYPED_TEST(StaticMapTest, InsertOverLimitExistingElements)
{
    uint32_t temp1{ 3215 };

    ASSERT_EQ(Insert(&this->container, temp1, 0), 1);

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(Insert(&this->container, temp1, i), ELEMENT_EXISTS);
    }
    ASSERT_EQ(Size(&this->container), 1);
}

static_map_impl(StaticMapV3, int, int, CONTAINER_CAPACITY);
dynamic_map_impl(DynamicMapV3, int, int);
static_unordered_map_impl(StaticUnorderedMapV3, int, int, CONTAINER_CAPACITY);
dynamic_unordered_map_impl(DynamicUnorderedMapV3, int, int);
