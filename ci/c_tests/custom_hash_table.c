#include "rtlib/memory.h"
#include "rtlib/hash_table.h"

/* Verify if compiles */
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

static unsigned int int_Hash(const int * value)
{
    return *value;
}

memory_t(DynamicAllocator);
dynamic_memory_t(DynamicAllocator);

hash_table_t(TestUnit, int);
custom_allocator_hash_table_t(TestUnit, int, DynamicAllocator);

int main()
{
    TestUnit test_unit;
    TestUnit_Construct(&test_unit);
    TestUnit_Destroy(&test_unit);

    return 0;
}