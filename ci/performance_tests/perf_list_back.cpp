#include "perf_common.h"
#include "perf_suites.h"

void run_list_back()
{
    MAKE_SUITE(LIST_BACK_TEST, 30000, list_back);
}
