#include "perf_common.h"
#include "perf_suites.h"

void run_map_find()
{
    MAKE_SUITE(MAP_FIND_TEST, 100000, map_find);
}
