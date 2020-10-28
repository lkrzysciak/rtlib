#include "gtest/gtest.h"
#include "containers/static_binary_tree.h"
#include "containers/static_hash_table.h"
#include <map>

#define CONTAINER_CAPACITY  10

declare_static_binary_tree_t(SetType, int, CONTAINER_CAPACITY);
define_static_binary_tree_t(SetType, int, CONTAINER_CAPACITY);
declare_static_hash_table_t(HashTable, int, CONTAINER_CAPACITY);
define_static_hash_table_t(HashTable, int, CONTAINER_CAPACITY);

int compare_set_ints(const int* v1, const int* v2)
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

unsigned int hash_function(const int* value)
{
    return *value;
}

#define create_wrappers_for_type(Type) \
void Deinit(Type* const container) \
{ \
    Type##_Destroy(container); \
} \
\
size_t Size(Type* const container) \
{ \
    return Type##_Size(container); \
} \
\
bool Empty(Type* const container) \
{ \
    return Type##_Empty(container); \
} \
\
int Insert(Type* const container, int value) \
{ \
    return Type##_Insert(container, value); \
} \
\
int Erase(Type* const container, Type##_iterator* it) \
{ \
    return Type##_Erase(container, it); \
} \
\
auto Begin(Type* const container) \
{ \
    return Type##_Begin(container); \
} \
\
auto End(Type* const container) \
{ \
    return Type##_End(container); \
} \
\
auto IteratorValue(Type##_iterator* const it) \
{ \
    return Type##_Iterator_GetValue(it); \
} \
\
void IteratorInc(Type##_iterator* const it) \
{ \
    return Type##_Iterator_Increment(it); \
} \
\
void IteratorDec(Type##_iterator* const it) \
{ \
    return Type##_Iterator_Decrement(it); \
} \
\
void IteratorSetValue(Type##_iterator* const it, int value) \
{ \
    Type##_Iterator_SetValue(it, value); \
} \
\
bool Iterator_Equal(Type##_iterator* const first, Type##_iterator* const second) \
{ \
    return Type##_Iterator_Equal(first, second); \
} \
\
auto Find(Type* const container, int value) \
{ \
    return Type##_Find(container, value); \
}

/* Specielized init functions */
void Init(SetType* const set_object)
{
    SetType_Construct(set_object, compare_set_ints);
}

void Init(HashTable* const hash_table)
{
    HashTable_Construct(hash_table, compare_set_ints, hash_function);
}

create_wrappers_for_type(SetType);
create_wrappers_for_type(HashTable);


template<typename T>
struct SetTest : public testing::Test
{};

template<typename T>
struct StaticSetTest : public testing::Test
{};

using MyTypes = testing::Types<
    SetType,
    HashTable
        >;

using StaticContainerTypes = testing::Types<
    SetType,
    HashTable
        >;

TYPED_TEST_CASE(SetTest, MyTypes);
TYPED_TEST_CASE(StaticSetTest, StaticContainerTypes);

TYPED_TEST(SetTest, Init)
{
    TypeParam container{};
    Init(&container);
}

TYPED_TEST(SetTest, IsEmptyAfterInit)
{
    TypeParam container{};
    Init(&container);

    ASSERT_EQ(Size(&container), 0);
    ASSERT_TRUE(Empty(&container));
}

TYPED_TEST(SetTest, InsertVerifyFromBeginToEnd)
{
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    std::set<int> expected_set{temp1, temp2, temp3, temp4, temp5};

    ASSERT_EQ(Insert(&container, temp1), 1);
    ASSERT_EQ(Insert(&container, temp2), 2);
    ASSERT_EQ(Insert(&container, temp3), 3);
    ASSERT_EQ(Insert(&container, temp4), 4);
    ASSERT_EQ(Insert(&container, temp5), 5);

    std::set<int> to_compare_set{};

    auto it = Begin(&container);
    auto end = End(&container);

    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set, to_compare_set);
    ASSERT_EQ(Size(&container), 5);
    ASSERT_FALSE(Empty(&container));
}

TYPED_TEST(SetTest, InsertVerifyFromEndToBegin)
{
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    std::set<int> expected_set{temp1, temp2, temp3, temp4, temp5};

    ASSERT_EQ(Insert(&container, temp1), 1);
    ASSERT_EQ(Insert(&container, temp2), 2);
    ASSERT_EQ(Insert(&container, temp3), 3);
    ASSERT_EQ(Insert(&container, temp4), 4);
    ASSERT_EQ(Insert(&container, temp5), 5);

    std::set<int> to_compare_set{};

    auto begin = Begin(&container);
    auto it = End(&container);

    while(!Iterator_Equal(&it, &begin))
    {   
        IteratorDec(&it);
        to_compare_set.insert(IteratorValue(&it));
    }    
    ASSERT_EQ(expected_set, to_compare_set);
    ASSERT_EQ(Size(&container), 5);
    ASSERT_FALSE(Empty(&container));
}

TYPED_TEST(SetTest, EraseVerifyFromBeginToEnd)
{
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&container, temp1);
    Insert(&container, temp2);
    Insert(&container, temp3);
    Insert(&container, temp4);
    Insert(&container, temp5);

    std::set<int> to_compare_set{};

    auto it = Begin(&container);
    auto end = End(&container);

    /* Remove temp4 ***************************************************/
    // Get temp4 iterator
    while(IteratorValue(&it) != temp4)
    {
        IteratorInc(&it);
    }

    // Remove temp4
    ASSERT_EQ(Erase(&container, &it), 4);

    std::set<int> expected_set_1{temp1, temp2, temp3, temp5};

    it = Begin(&container);
    end = End(&container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set_1, to_compare_set);
    ASSERT_EQ(Size(&container), 4);

    /* Remove temp2 ***************************************************/
    it = Begin(&container);
    end = End(&container);

    while(IteratorValue(&it) != temp2)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&container, &it), 3);

    std::set<int> expected_set_2{temp1, temp3, temp5};

    it = Begin(&container);
    end = End(&container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set_2, to_compare_set);
    ASSERT_EQ(Size(&container), 3);

    /* Remove temp1 ***************************************************/
    it = Begin(&container);
    end = End(&container);
    
    while(IteratorValue(&it) != temp1)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&container, &it), 2);

    std::set<int> expected_set_3{temp3, temp5};

    it = Begin(&container);
    end = End(&container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set_3, to_compare_set);
    ASSERT_EQ(Size(&container), 2);

    /* Remove temp5 ***************************************************/
    it = Begin(&container);
    end = End(&container);
    
    while(IteratorValue(&it) != temp5)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&container, &it), 1);

    std::set<int> expected_set_4{temp3};

    it = Begin(&container);
    end = End(&container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set_4, to_compare_set);
    ASSERT_EQ(Size(&container), 1);

    /* Remove temp3 ***************************************************/
    it = Begin(&container);
    end = End(&container);
    
    while(IteratorValue(&it) != temp3)
    {
        IteratorInc(&it);
    }

    ASSERT_EQ(Erase(&container, &it), 0);

    std::set<int> expected_set_5{};

    it = Begin(&container);
    end = End(&container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set_5, to_compare_set);
    ASSERT_EQ(Size(&container), 0);

    /* Insert temp6 ****************************************/
    int temp6{ 4321 };
    ASSERT_EQ(Insert(&container, temp6), 1);
    std::set<int> expected_set_6{temp6};

    it = Begin(&container);
    end = End(&container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set_6, to_compare_set);
    ASSERT_EQ(Size(&container), 1);
}

TYPED_TEST(SetTest, ModifyContainerValues)
{
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&container, temp1);
    Insert(&container, temp2);
    Insert(&container, temp3);
    Insert(&container, temp4);
    Insert(&container, temp5);

    std::set<int> to_compare_set{};

    auto it = Begin(&container);
    auto end = End(&container);

    /* Change temp4 value */
    while(IteratorValue(&it) != temp4)
    {
        IteratorInc(&it);
    }

    int temp4_1{ 741 };

    IteratorSetValue(&it, temp4_1);

    std::set<int> expected_set_1{temp1, temp2, temp3, temp4_1, temp5};

    it = Begin(&container);
    end = End(&container);
    to_compare_set.clear();
    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set_1, to_compare_set);

    /* Change temp1 value */
    it = Begin(&container);
    end = End(&container);
    while(IteratorValue(&it) != temp1)
    {
        IteratorInc(&it);
    }

    int temp1_1{ 8520 };

    IteratorSetValue(&it, temp1_1);

    std::set<int> expected_set_2{temp1_1, temp2, temp3, temp4_1, temp5};

    it = Begin(&container);
    end = End(&container);
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
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&container, temp1);
    Insert(&container, temp2);
    Insert(&container, temp3);
    Insert(&container, temp4);
    Insert(&container, temp5);

    auto temp1It = Find(&container, temp1);
    ASSERT_EQ(IteratorValue(&temp1It), temp1);

    auto temp2It = Find(&container, temp2);
    ASSERT_EQ(IteratorValue(&temp2It), temp2);

    auto temp3It = Find(&container, temp3);
    ASSERT_EQ(IteratorValue(&temp3It), temp3);

    auto temp4It = Find(&container, temp4);
    ASSERT_EQ(IteratorValue(&temp4It), temp4);

    auto temp5It = Find(&container, temp5);
    ASSERT_EQ(IteratorValue(&temp5It), temp5);
}

TYPED_TEST(SetTest, FindNonExistedMember)
{
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };
    int temp6{ 753 };

    Insert(&container, temp1);
    Insert(&container, temp2);
    Insert(&container, temp3);
    Insert(&container, temp4);
    Insert(&container, temp5);

    auto end = End(&container);

    auto temp6It = Find(&container, temp6);
    ASSERT_TRUE(Iterator_Equal(&temp6It, &end));
}

TYPED_TEST(SetTest, IncrementAndDecrementIterator)
{
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };
    int temp4{ 1024 };
    int temp5{ 5005 };

    Insert(&container, temp1);
    Insert(&container, temp2);
    Insert(&container, temp3);
    Insert(&container, temp4);
    Insert(&container, temp5);

    auto it = Begin(&container);

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
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };

    ASSERT_EQ(Insert(&container, temp1), 1);
    ASSERT_EQ(Insert(&container, temp1), ELEMENT_EXISTS);
}

TYPED_TEST(StaticSetTest, InsertOverLimit)
{
    TypeParam container{};
    Init(&container);

    uint32_t temp1{ 3215 };

    for(int i=0; i<CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(Insert(&container, temp1 + i), i + 1);
    }
    ASSERT_EQ(Insert(&container, temp1), ALLOCATION_ERROR);
}