#include "gtest/gtest.h"
#include "containers/static_set.h"
#include <map>

declare_static_set_t(SetType, int, 10);
define_static_set_t(SetType, int, 10);

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

void Init(SetType* const set_object)
{
    SetType_Init(set_object, compare_set_ints);
}

size_t Size(SetType* const set_object)
{
    return SetType_Size(set_object);
}

bool Empty(SetType* const set_object)
{
    return SetType_Empty(set_object);
}

int Insert(SetType* const set_object, int value)
{
    return SetType_Insert(set_object, value);
}

template<typename iterator_t>
int Erase(SetType* const set_object, iterator_t* it)
{
    return SetType_Erase(set_object, it);
}

auto Begin(SetType* const set_object)
{
    return SetType_Begin(set_object);
}

auto End(SetType* const set_object)
{
    return SetType_End(set_object);
}

auto IteratorValue(SetType_iterator* const it)
{
    return SetType_Iterator_GetValue(it);
}

void IteratorInc(SetType_iterator* const it)
{
    return SetType_Iterator_Increment(it);
}

void IteratorDec(SetType_iterator* const it)
{
    return SetType_Iterator_Decrement(it);
}

void IteratorSetValue(SetType_iterator* const it, int value)
{
    SetType_Iterator_SetValue(it, value);
}

bool Iterator_Equal(SetType_iterator* const first, SetType_iterator* const second)
{
    return SetType_Iterator_Equal(first, second);
}

auto Find(SetType* const vector, int value)
{
    return SetType_Find(vector, value);
}

template<typename T>
struct SetTest : public testing::Test
{};

using MyTypes = testing::Types<
    SetType
        >;
TYPED_TEST_CASE(SetTest, MyTypes);

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

TYPED_TEST(SetTest, Insert)
{
    TypeParam container{};
    Init(&container);

    int temp1{ 3215 };
    int temp2{ 23587 };
    int temp3{ 980 };

    std::set<int> expected_set{temp1, temp2, temp3};

    ASSERT_EQ(Insert(&container, temp1), 1);
    ASSERT_EQ(Insert(&container, temp2), 2);
    ASSERT_EQ(Insert(&container, temp3), 3);

    printf("%d %d %d\n", container.root->left->value, container.root->value, container.root->right->value);

    std::set<int> to_compare_set{};

    auto it = Begin(&container);
    auto end = End(&container);

    while(!Iterator_Equal(&it, &end))
    {   
        to_compare_set.insert(IteratorValue(&it));
        IteratorInc(&it);
    }    
    ASSERT_EQ(expected_set, to_compare_set);
    ASSERT_EQ(Size(&container), 3);
    ASSERT_FALSE(Empty(&container));
}