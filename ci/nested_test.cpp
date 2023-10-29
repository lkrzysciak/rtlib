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

    String key1Str{};
    String_Construct2(&key1Str, key1, strlen(key1));

    String key2Str{};
    String_Construct2(&key2Str, key2, strlen(key2));

    String key3Str{};
    String_Construct2(&key3Str, key3, strlen(key3));

    DMap_Insert(&dmap, key1Str, 10);
    DMap_Insert(&dmap, key2Str, 20);

    ASSERT_EQ(*DMap_CRef(&dmap, key1Str), 10);
    ASSERT_EQ(*DMap_CRef(&dmap, key2Str), 20);
    ASSERT_EQ(DMap_CRef(&dmap, key3Str), nullptr);

    DMap_Destruct(&dmap);
}

dynamic_string_impl(String);
dynamic_unordered_map_impl(DMap, String, int);