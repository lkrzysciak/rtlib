#include "perf_common.h"
#include "perf_suites.h"

void run_unordered_map_find()
{
    MAKE_SUITE(UNORDERED_MAP_FIND_TEST, 100000, unordered_map_find);
}
