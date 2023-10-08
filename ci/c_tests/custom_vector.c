#include "rtlib/memory.h"
#include "rtlib/vector.h"

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

memory_t(DynamicAllocator);
dynamic_memory_t(DynamicAllocator);

vector_t(TestUnit, int);
custom_allocator_vector_t(TestUnit, int, DynamicAllocator);

int main()
{
    TestUnit test_unit;
    TestUnit_Construct(&test_unit);
    TestUnit_Destroy(&test_unit);

    return 0;
}