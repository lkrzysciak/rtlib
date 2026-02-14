#include "perf_common.h"
#include "perf_suites.h"

void run_heap_malloc_free()
{
    MAKE_SUITE(HEAP_MALLOC_FREE_TEST, 2000, heap_malloc_free);
}
