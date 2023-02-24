#include "rtlib/event_loop.h"

/* Verify if compiles */

memory_t(DynamicAllocator);
dynamic_memory_t(DynamicAllocator);

event_loop_t(TestUnit, int);
custom_allocator_event_loop_t(TestUnit, int, DynamicAllocator);

event_loop_t(TestUnit2, void *);
custom_allocator_event_loop_t(TestUnit2, void *, DynamicAllocator);

static int test_value;

void test_func(int val)
{
    test_value = val;
}

void test_func2(void * val)
{
    test_value = *(int *)val;
}

int main()
{
    int val1 = 3;
    int val2 = 5;
    int val3 = 1;

    TestUnit test_unit;
    TestUnit_Construct(&test_unit);

    TestUnit_Enqueue(&test_unit, test_func, val1);
    TestUnit_Enqueue(&test_unit, test_func, val2);
    TestUnit_Enqueue(&test_unit, test_func, val3);

    assert(test_value == 0);

    TestUnit_Process(&test_unit);
    assert(test_value == val1);

    TestUnit_Process(&test_unit);
    assert(test_value == val2);

    TestUnit_Process(&test_unit);
    assert(test_value == val3);

    TestUnit_Destroy(&test_unit);

    TestUnit2 test_unit2;
    TestUnit2_Construct(&test_unit2);

    TestUnit2_Enqueue(&test_unit2, test_func2, &val1);
    TestUnit2_Enqueue(&test_unit2, test_func2, &val2);
    TestUnit2_Enqueue(&test_unit2, test_func2, &val3);

    TestUnit2_Process(&test_unit2);
    assert(test_value == val1);

    TestUnit2_Process(&test_unit2);
    assert(test_value == val2);

    TestUnit2_Process(&test_unit2);
    assert(test_value == val3);

    TestUnit2_Destroy(&test_unit2);

    return 0;
}