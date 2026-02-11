#include "perf_common.h"
#include "perf_suites.h"

void run_set_find()
{
    MAKE_SUITE(SET_FIND_TEST, 100000, set_find);
}
