#include "gtest/gtest.h"
#include <cstdint>
#include <tuple>
#include "rtlib/string.h"
#include "rtlib/unordered_map.h"

dynamic_string(String);
dynamic_unordered_map(DMap, String, int);

TEST(StringAsKey, Test1)
{
    const char * key1 = "Hello";
    const char * key2 = "world";
    const char * key3 = "not exist";

    DMap dmap{};
    DMap_Construct(&dmap);

    String key1Str, key2Str, key3Str;
    String_Construct3(&key1Str, key1);
    String_Construct3(&key2Str, key2);
    String_Construct3(&key3Str, key3);

    DMap_Insert(&dmap, key1Str, 10);
    DMap_Insert(&dmap, key2Str, 20);

    ASSERT_EQ(*DMap_CRef(&dmap, key1Str), 10);
    ASSERT_EQ(*DMap_CRef(&dmap, key2Str), 20);
    ASSERT_EQ(DMap_CRef(&dmap, key3Str), nullptr);

    DMap_Destruct(&dmap);

    String_Destruct(&key1Str);
    String_Destruct(&key2Str);
    String_Destruct(&key3Str);
}

dynamic_string_impl(String);
dynamic_unordered_map_impl(DMap, String, int);