#include "perf_common.h"
#include "perf_suites.h"

void run_list_middle()
{
    MAKE_SUITE(LIST_MIDDLE_TEST, 30000, list_middle);
}
