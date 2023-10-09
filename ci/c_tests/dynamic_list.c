#include "rtlib/list.h"

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

list_t(TestUnit, int);
dynamic_list_t(TestUnit, int);

int main()
{
    TestUnit test_unit;
    TestUnit_Construct(&test_unit);
    TestUnit_PushBack(&test_unit, 5);
    TestUnit_Destruct(&test_unit);

    return 0;
}