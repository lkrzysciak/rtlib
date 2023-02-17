#include "rtlib/memory.h"
#include "rtlib/list.h"

/* Verify if compiles */
memory_t(DynamicAllocator);
dynamic_memory_t(DynamicAllocator);

list_t(TestUnit, int);
custom_allocator_list_t(TestUnit, int, DynamicAllocator);

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

int main()
{
    TestUnit test_unit;
    TestUnit_Construct(&test_unit, compare_set_ints);
    TestUnit_Destroy(&test_unit);

    return 0;
}