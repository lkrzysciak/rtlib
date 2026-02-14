#include "perf_common.h"
#include "perf_suites.h"

void run_heap_realloc_free()
{
    MAKE_SUITE(HEAP_REALLOC_FREE_TEST, 30000, heap_realloc_free);
}
