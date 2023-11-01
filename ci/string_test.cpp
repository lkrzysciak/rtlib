#include "gtest/gtest.h"
#include <cstdint>
#include <tuple>
#include "rtlib/string.h"

#define create_wrappers_for_type(Type)                                                 \
                                                                                       \
    void Init(Type * const container)                                                  \
    {                                                                                  \
        Type##_Construct(container);                                                   \
    }                                                                                  \
                                                                                       \
    void Init2(Type * const container, const char * data, size_t size)                 \
    {                                                                                  \
        Type##_Construct2(container, data, size);                                      \
    }                                                                                  \
                                                                                       \
    void Init3(Type * const container, const char * data)                              \
    {                                                                                  \
        Type##_Construct3(container, data);                                            \
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
    int PushBack(Type * const container, char value)                                   \
    {                                                                                  \
        return Type##_PushBack(container, value);                                      \
    }                                                                                  \
                                                                                       \
    int PushFront(Type * const container, char value)                                  \
    {                                                                                  \
        return Type##_PushFront(container, value);                                     \
    }                                                                                  \
                                                                                       \
    int Insert(Type * const container, char value, Type##_Iterator * it)               \
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
    char Back(Type * const container)                                                  \
    {                                                                                  \
        return Type##_Back(container);                                                 \
    }                                                                                  \
                                                                                       \
    char Front(Type * const container)                                                 \
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
    auto Find(Type * const container, char value)                                      \
    {                                                                                  \
        return Type##_Find(container, value);                                          \
    }                                                                                  \
                                                                                       \
    void Clear(Type * const container)                                                 \
    {                                                                                  \
        Type##_Clear(container);                                                       \
    }                                                                                  \
                                                                                       \
    int Compare(const Type * const container, const Type * const second)               \
    {                                                                                  \
        return Type##_Compare(container, second);                                      \
    }                                                                                  \
                                                                                       \
    unsigned int Hash(Type * const container)                                          \
    {                                                                                  \
        return Type##_Hash(container);                                                 \
    }

#define CONTAINER_CAPACITY 100

static_string(SString, CONTAINER_CAPACITY);
dynamic_string(DString);

create_wrappers_for_type(SString);
create_wrappers_for_type(DString);

template<typename T>
struct StringNotInitializedTest : public testing::Test
{
    T container;
};

template<typename T>
struct StringTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;

    using MyParamType = T;
};

template<typename T>
struct StaticStringTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

template<typename T>
struct DynamicStringTest : public testing::Test
{
    void SetUp() override { Init(&container); }

    void TearDown() override { Deinit(&container); }

    T container;
};

using AllStringTypes     = testing::Types<SString, DString>;
using StaticStringTypes  = testing::Types<SString>;
using DynamicStringTypes = testing::Types<DString>;

TYPED_TEST_SUITE(StringNotInitializedTest, AllStringTypes);
TYPED_TEST_SUITE(StringTest, AllStringTypes);
TYPED_TEST_SUITE(StaticStringTest, StaticStringTypes);
TYPED_TEST_SUITE(DynamicStringTest, DynamicStringTypes);

TYPED_TEST(StringNotInitializedTest, WithValue)
{
    const char data[] = "Hello world";
    Init2(&this->container, data, 11);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), 11);
    ASSERT_EQ(strcmp(Ref(&this->container, 0), data), 0);
}

TYPED_TEST(StringNotInitializedTest, WithValue2)
{
    const char data[] = "Hello world";
    Init3(&this->container, data);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), 11);
    ASSERT_EQ(strcmp(Ref(&this->container, 0), data), 0);
}

TYPED_TEST(StringTest, IsEmptyAfterInit)
{
    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, PushBack)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

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

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, PushFront)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

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

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, Insert)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

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

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, Erase)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

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

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&this->container, &it), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));

    it = Begin(&this->container);
    IteratorInc(&it);
    ASSERT_EQ(Erase(&this->container, &it), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));

    it = Begin(&this->container);
    ASSERT_EQ(Erase(&this->container, &it), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, PopBack)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));

    ASSERT_EQ(PopBack(&this->container), 1);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp1);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));

    ASSERT_EQ(PopBack(&this->container), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, PopFront)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    ASSERT_EQ(PushBack(&this->container, temp1), 1);
    ASSERT_EQ(PushBack(&this->container, temp2), 2);
    ASSERT_EQ(Front(&this->container), temp1);
    ASSERT_EQ(Back(&this->container), temp2);
    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));

    ASSERT_EQ(PopFront(&this->container), 1);
    ASSERT_EQ(Front(&this->container), temp2);
    ASSERT_EQ(Back(&this->container), temp2);
    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));

    ASSERT_EQ(PopFront(&this->container), 0);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_TRUE(Empty(&this->container));
    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, BeginEndIterator)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it       = End(&this->container);

    IteratorDec(&it);

    ASSERT_EQ(*Iterator_CRef(&it_begin), temp1);
    ASSERT_EQ(*Iterator_CRef(&it), temp2);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, IteratorIncrementation)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

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

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, IteratorDecrementation)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

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
    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, IteratorAfterInit)
{
    auto it_begin = Begin(&this->container);
    auto it_end   = End(&this->container);

    ASSERT_TRUE(Iterator_Equal(&it_begin, &it_end));
}

TYPED_TEST(StringTest, Iterator_CRefs)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    auto it_begin = Begin(&this->container);
    auto it_end   = End(&this->container);

    auto it = it_end;
    IteratorDec(&it);
    char newTemp2{ 'd' };
    *Iterator_Ref(&it) = newTemp2;
    ASSERT_EQ(*Iterator_CRef(&it), newTemp2);
    IteratorDec(&it);
    char newTemp1{ 'e' };
    *Iterator_Ref(&it) = newTemp1;
    ASSERT_EQ(*Iterator_CRef(&it), newTemp1);
    ASSERT_TRUE(Iterator_Equal(&it_begin, &it));

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, IndexValues)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(*CRef(&this->container, 0), temp1);
    ASSERT_EQ(*CRef(&this->container, 1), temp2);

    char newTemp1{ 'd' };
    char newTemp2{ 'e' };

    *Ref(&this->container, 0) = newTemp1;
    ASSERT_EQ(*CRef(&this->container, 0), newTemp1);
    ASSERT_EQ(*CRef(&this->container, 1), temp2);

    *Ref(&this->container, 1) = newTemp2;
    ASSERT_EQ(*CRef(&this->container, 0), newTemp1);
    ASSERT_EQ(*CRef(&this->container, 1), newTemp2);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, Ref)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(*Ref(&this->container, 0), temp1);
    ASSERT_EQ(*Ref(&this->container, 1), temp2);

    char newTemp1{ 'd' };
    char newTemp2{ 'e' };

    *Ref(&this->container, 0) = newTemp1;
    ASSERT_EQ(*Ref(&this->container, 0), newTemp1);
    ASSERT_EQ(*Ref(&this->container, 1), temp2);

    *Ref(&this->container, 1) = newTemp2;
    ASSERT_EQ(*Ref(&this->container, 0), newTemp1);
    ASSERT_EQ(*Ref(&this->container, 1), newTemp2);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, CRef)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);

    ASSERT_EQ(*CRef(&this->container, 0), temp1);
    ASSERT_EQ(*CRef(&this->container, 1), temp2);

    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, FindAllValues)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

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

TYPED_TEST(StringTest, Clear)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);
    ASSERT_EQ(Size(&this->container), 3);

    Clear(&this->container);

    ASSERT_EQ(Size(&this->container), 0);
    ASSERT_EQ(strlen(Ref(&this->container, 0)), Size(&this->container));
}

TYPED_TEST(StringTest, CompareNE)
{
    using MyParamType = typename TestFixture::MyParamType;

    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);

    MyParamType second{};
    Init(&second);

    ASSERT_NE(Compare(&this->container, &second), 0);
}

TYPED_TEST(StringTest, CompareEQ)
{
    using MyParamType = typename TestFixture::MyParamType;

    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);

    MyParamType second{};
    Init2(&second, "abc", 3);

    ASSERT_EQ(Compare(&this->container, &second), 0);
}

TYPED_TEST(StringTest, Hash)
{
    char temp1{ 'a' };
    char temp2{ 'b' };
    char temp3{ 'c' };

    PushBack(&this->container, temp1);
    PushBack(&this->container, temp2);
    PushBack(&this->container, temp3);

    ASSERT_EQ(Hash(&this->container), 193485963);
}

TYPED_TEST(StaticStringTest, PushBackOverLimit)
{
    char temp1{ 'a' };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushBack(&this->container, temp1), i + 1);
    }
    ASSERT_EQ(PushBack(&this->container, temp1), ALLOCATION_ERROR);
    ASSERT_EQ(strlen(Ref(&this->container, 0)), CONTAINER_CAPACITY);
}

TYPED_TEST(StaticStringTest, PushFrontOverLimit)
{
    char temp1{ 'a' };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        ASSERT_EQ(PushFront(&this->container, temp1), i + 1);
    }
    ASSERT_EQ(PushFront(&this->container, temp1), ALLOCATION_ERROR);
    ASSERT_EQ(strlen(Ref(&this->container, 0)), CONTAINER_CAPACITY);
}

TYPED_TEST(StaticStringTest, InsertOverLimit)
{
    char temp1{ 'a' };

    for(int i = 0; i < CONTAINER_CAPACITY; ++i)
    {
        auto it = Begin(&this->container);
        ASSERT_EQ(Insert(&this->container, temp1, &it), i + 1);
    }
    auto it = Begin(&this->container);
    ASSERT_EQ(Insert(&this->container, temp1, &it), ALLOCATION_ERROR);
    ASSERT_EQ(strlen(Ref(&this->container, 0)), CONTAINER_CAPACITY);
}

TYPED_TEST(DynamicStringTest, AddALotOfElementsToMakeManyReallocations)
{
    char temp1{ 'a' };

    for(int i = 0; i < 1000; ++i)
    {
        auto it = Begin(&this->container);
        ASSERT_EQ(Insert(&this->container, temp1, &it), i + 1);
        ASSERT_EQ(Front(&this->container), temp1);

        ASSERT_EQ(strlen(Ref(&this->container, 0)), i + 1);
    }
}

static_string_impl(SString, CONTAINER_CAPACITY);
dynamic_string_impl(DString);