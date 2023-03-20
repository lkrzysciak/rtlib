#include "rtlib/memory.h"
#include "rtlib/hash_table.h"

/* Verify if compiles */
memory_t(DynamicAllocator);
dynamic_memory_t(DynamicAllocator);

hash_table_t(TestUnit, int);
custom_allocator_hash_table_t(TestUnit, int, DynamicAllocator);

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

static unsigned int hash_function(const int * value)
{
    return *value;
}

int main()
{
    TestUnit test_unit;
    TestUnit_Construct(&test_unit, compare_set_ints, hash_function);
    TestUnit_Destroy(&test_unit);

    return 0;
}