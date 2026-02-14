#include "perf_common.h"
#include "perf_suites.h"

void run_heap_calloc_free()
{
    MAKE_SUITE(HEAP_CALLOC_FREE_TEST, 20000, heap_calloc_free);
}
