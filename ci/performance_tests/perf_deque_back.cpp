#include "perf_common.h"
#include "perf_suites.h"

void run_deque_back()
{
    MAKE_SUITE(DEQUE_BACK_TEST, 300000, deque_back);
}
