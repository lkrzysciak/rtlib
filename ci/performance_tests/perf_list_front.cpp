#include "perf_common.h"
#include "perf_suites.h"

void run_list_front()
{
    MAKE_SUITE(LIST_FRONT_TEST, 30000, list_front);
}
