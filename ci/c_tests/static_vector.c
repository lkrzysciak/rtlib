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

vector_t(TestUnit, int);
static_vector_t(TestUnit, int, 20);

int main()
{
    TestUnit test_unit;
    TestUnit_Construct(&test_unit);
    TestUnit_PushBack(&test_unit, 5);
    TestUnit_Destroy(&test_unit);

    return 0;
}