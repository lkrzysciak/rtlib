#include "gtest/gtest.h"
#include "containers/static_binary_tree.h"
#include "containers/static_hash_table.h"
#include "containers/custom_allocator_binary_tree.h"
#include "containers/custom_allocator_hash_table.h"
#include "containers/dynamic_binary_tree.h"
#include "containers/dynamic_hash_table.h"
#include "memory/dynamic_allocator.h"
#include <map>
#include <set>
#include <list>
#include <numeric>

#define CONTAINER_CAPACITY 100

extern "C"
{
    declare_static_binary_tree_t(SetType, int, CONTAINER_CAPACITY);
    define_static_binary_tree_t(SetType, int, CONTAINER_CAPACITY);
    declare_static_hash_table_t(HashTable, int, CONTAINER_CAPACITY);
    define_static_hash_table_t(HashTable, int, CONTAINER_CAPACITY);

    declare_dynamic_allocator_t(MyDynamicAllocator);
    define_dynamic_allocator_t(MyDynamicAllocator);
    declare_custom_allocator_binary_tree_t(CustomBinaryTree, int, MyDynamicAllocator);
    define_custom_allocator_binary_tree_t(CustomBinaryTree, int, MyDynamicAllocator);
    declare_custom_allocator_hash_table_t(CustomHashTable, int, MyDynamicAllocator);
    define_custom_allocator_hash_table_t(CustomHashTable, int, MyDynamicAllocator);

    declare_static_binary_tree_t(SSetTypeWithPointers, int *, CONTAINER_CAPACITY);
    define_static_binary_tree_t(SSetTypeWithPointers, int *, CONTAINER_CAPACITY);
    declare_static_hash_table_t(SHashTableWithPointers, int *, CONTAINER_CAPACITY);
    define_static_hash_table_t(SHashTableWithPointers, int *, CONTAINER_CAPACITY);
    declare_custom_allocator_binary_tree_t(CSetTypeWithPointers, int *, MyDynamicAllocator);
    define_custom_allocator_binary_tree_t(CSetTypeWithPointers, int *, MyDynamicAllocator);
    declare_custom_allocator_hash_table_t(CHashTableWithPointers, int *, MyDynamicAllocator);
    define_custom_allocator_hash_table_t(CHashTableWithPointers, int *, MyDynamicAllocator);

    declare_dynamic_binary_tree_t(DynamicBinaryTree, int);
    define_dynamic_binary_tree_t(DynamicBinaryTree, int);
    declare_dynamic_hash_table_t(DynamicHashTable, int);
    define_dynamic_hash_table_t(DynamicHashTable, int);

    typedef struct
    {
        double doubleVar;
        int intVar;
        bool boolVar;
        uint64_t id;
    } StructType;

    declare_static_binary_tree_t(StructTypeStaticBinaryTree, StructType, CONTAINER_CAPACITY);
    define_static_binary_tree_t(StructTypeStaticBinaryTree, StructType, CONTAINER_CAPACITY);
    declare_static_hash_table_t(StructTypeStaticHashTable, StructType, CONTAINER_CAPACITY);
    define_static_hash_table_t(StructTypeStaticHashTable, StructType, CONTAINER_CAPACITY);
    declare_dynamic_binary_tree_t(StructTypeDynamicBinaryTree, StructType);
    define_dynamic_binary_tree_t(StructTypeDynamicBinaryTree, StructType);
    declare_dynamic_hash_table_t(StructTypeDynamicHashTable, StructType);
    define_dynamic_hash_table_t(StructTypeDynamicHashTable, StructType);
}

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

static unsigned int hash_function(const int * value)
{
    return *value;
}

static unsigned int hash_function_ptr(const int ** value)
{
    return **value;
}

static unsigned int hash_function_struct_type(const StructType * value)
{
    return value->id;
}

#define create_wrappers_for_type(Type, MemberType)                                                               \
    void Deinit(Type * const container) { Type##_Destroy(container); }                                           \
                                                                                                                 \
    size_t Size(Type * const container) { return Type##_Size(container); }                                       \
                                                                                                                 \
    bool Empty(Type * const container) { return Type##_Empty(container); }                                       \
                                                                                                                 \
    int Insert(Type * const container, MemberType value) { return Type##_Insert(container, value); }             \
                                                                                                                 \
    int Erase(Type * const container, Type##_Iterator * it) { return Type##_Erase(container, it); }              \
                                                                                                                 \
    auto Begin(Type * const container) { return Type##_Begin(container); }                                       \
                                                                                                                 \
    auto End(Type * const container) { return Type##_End(container); }                                           \
                                                                                                                 \
    auto IteratorValue(Type##_Iterator * const it) { return Type##_Iterator_GetValue(it); }                      \
                                                                                                                 \
    void IteratorInc(Type##_Iterator * const it) { return Type##_Iterator_Increment(it); }                       \
                                                                                                                 \
    void IteratorDec(Type##_Iterator * const it) { return Type##_Iterator_Decrement(it); }                       \
                                                                                                                 \
    void IteratorSetValue(Type##_Iterator * const it, MemberType value) { Type##_Iterator_SetValue(it, value); } \
                                                                                                                 \
    bool Iterator_Equal(Type##_Iterator * const first, Type##_Iterator * const second)                           \
    {                                                                                                            \
        return Type##_Iterator_Equal(first, second);                                                             \
    }                                                                                                            \
                                                                                                                 \
    auto Find(Type * const container, MemberType value) { return Type##_Find(container, value); }                \
                                                                                                                 \
    auto CustomFind(Type * const container, MemberType value, Type##_compare_t custom_comparator)                \
    {                                                                                                            \
        return Type##_CustomFind(container, value, custom_comparator);                                           \
    }                                                                                                            \
                                                                                                                 \
    void Clear(Type * const container) { Type##_Clear(container); }

/* Specialized init functions */
void Init(SetType * const set_object)
{
    SetType_Construct(set_object, compare_set_ints);
}

void Init(SSetTypeWithPointers * const set_object)
{
    SSetTypeWithPointers_Construct(set_object, compare_set_ints_ptr);
}

void Init(HashTable * const hash_table)
{
    HashTable_Construct(hash_table, compare_set_ints, hash_function);
}

void Init(SHashTableWithPointers * const hash_table)
{
    SHashTableWithPointers_Construct(hash_table, compare_set_ints_ptr, hash_function_ptr);
}

void Init(CustomBinaryTree * const container)
{
    CustomBinaryTree_Construct(container, compare_set_ints);
}

void Init(CustomHashTable * const hash_table)
{
    CustomHashTable_Construct(hash_table, compare_set_ints, hash_function);
}

void Init(DynamicBinaryTree * const container)
{
    DynamicBinaryTree_Construct(container, compare_set_ints);
}

void Init(DynamicHashTable * const hash_table)
{
    DynamicHashTable_Construct(hash_table, compare_set_ints, hash_function);
}

create_wrappers_for_type(SetType, int);
create_wrappers_for_type(HashTable, int);
create_wrappers_for_type(CustomBinaryTree, int);
create_wrappers_for_type(CustomHashTable, int);
create_wrappers_for_type(DynamicBinaryTree, int);
create_wrappers_for_type(DynamicHashTable, int);

create_wrappers_for_type(StructTypeStaticBinaryTree, StructType);
create_wrappers_for_type(StructTypeStaticHashTable, StructType);
create_wrappers_for_type(StructTypeDynamicBinaryTree, StructType);
create_wrappers_for_type(StructTypeDynamicHashTable, StructType);

void Init(StructTypeStaticBinaryTree * const container)
{
    StructTypeStaticBinaryTree_Construct(container, compare_struct_type);
}

void Init(StructTypeStaticHashTable * const container)
{
    StructTypeStaticHashTable_Construct(container, compare_struct_type, hash_function_struct_type);
}

void Init(StructTypeDynamicBinaryTree * const container)
{
    StructTypeDynamicBinaryTree_Construct(container, compare_struct_type);
}

void Init(StructTypeDynamicHashTable * const container)
{
    StructTypeDynamicHashTable_Construct(container, compare_struct_type, hash_function_struct_type);
}

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
struct SetStructTypeTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

using MyTypes =
    testing::Types<SetType, HashTable, CustomBinaryTree, CustomHashTable, DynamicBinaryTree, DynamicHashTable>;

using StaticContainerTypes = testing::Types<SetType, HashTable>;

using StructContainerTypes = testing::Types<StructTypeStaticBinaryTree, StructTypeStaticHashTable,
                                            StructTypeDynamicBinaryTree, StructTypeDynamicHashTable>;

TYPED_TEST_CASE(SetTest, MyTypes);
TYPED_TEST_CASE(StaticSetTest, StaticContainerTypes);
TYPED_TEST_CASE(SetStructTypeTest, StructContainerTypes);

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
        to_compare_set.insert(IteratorValue(&it));
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
        to_compare_set.insert(IteratorValue(&it));
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
    while(IteratorValue(&it) != temp4)
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
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_1, to_compare_set);
    ASSERT_EQ(Size(&this->container), 4);

    /* Remove temp2 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(IteratorValue(&it) != temp2)
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
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_2, to_compare_set);
    ASSERT_EQ(Size(&this->container), 3);

    /* Remove temp1 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(IteratorValue(&it) != temp1)
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
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_3, to_compare_set);
    ASSERT_EQ(Size(&this->container), 2);

    /* Remove temp5 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(IteratorValue(&it) != temp5)
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
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_4, to_compare_set);
    ASSERT_EQ(Size(&this->container), 1);

    /* Remove temp3 ***************************************************/
    it  = Begin(&this->container);
    end = End(&this->container);

    while(IteratorValue(&it) != temp3)
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
        to_compare_set.insert(IteratorValue(&it));
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
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_6, to_compare_set);
    ASSERT_EQ(Size(&this->container), 1);
}

TYPED_TEST(SetTest, ModifyContainerValues)
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

    /* Change temp4 value */
    while(IteratorValue(&it) != temp4)
    {
        IteratorInc(&it);
    }

    int temp4_1{ 741 };

    IteratorSetValue(&it, temp4_1);

    std::set<int> expected_set_1{ temp1, temp2, temp3, temp4_1, temp5 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_1, to_compare_set);

    /* Change temp1 value */
    it  = Begin(&this->container);
    end = End(&this->container);
    while(IteratorValue(&it) != temp1)
    {
        IteratorInc(&it);
    }

    int temp1_1{ 8520 };

    IteratorSetValue(&it, temp1_1);

    std::set<int> expected_set_2{ temp1_1, temp2, temp3, temp4_1, temp5 };

    it  = Begin(&this->container);
    end = End(&this->container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }
    ASSERT_EQ(expected_set_2, to_compare_set);
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
    ASSERT_EQ(IteratorValue(&temp1It), temp1);

    auto temp2It = Find(&this->container, temp2);
    ASSERT_EQ(IteratorValue(&temp2It), temp2);

    auto temp3It = Find(&this->container, temp3);
    ASSERT_EQ(IteratorValue(&temp3It), temp3);

    auto temp4It = Find(&this->container, temp4);
    ASSERT_EQ(IteratorValue(&temp4It), temp4);

    auto temp5It = Find(&this->container, temp5);
    ASSERT_EQ(IteratorValue(&temp5It), temp5);
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

TYPED_TEST(SetTest, CustomFinder)
{
    uint32_t temp1{ 3215 };
    uint32_t temp2{ 23587 };
    uint32_t temp3{ 582 };

    Insert(&this->container, temp1);
    Insert(&this->container, temp2);
    Insert(&this->container, temp3);

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

    auto it1Value = IteratorValue(&it);

    IteratorInc(&it);
    IteratorDec(&it);
    auto it1_1Value = IteratorValue(&it);
    ASSERT_EQ(it1Value, it1_1Value);

    IteratorDec(&it);
    IteratorInc(&it);
    auto it1_2Value = IteratorValue(&it);
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
        receivedSet.insert(IteratorValue(&it));
    }
    ASSERT_EQ(expectedSet, receivedSet);
}

TYPED_TEST(StaticSetTest, InsertOverLimit)
{
    uint32_t temp1{ 3215 };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(Insert(&this->container, temp1 + i), i + 1);
    }
    ASSERT_EQ(Insert(&this->container, temp1), ALLOCATION_ERROR);
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

    // Set test do have random queue of elements. (We know the internal implementation so we know order)
    auto it           = Begin(&this->container);
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