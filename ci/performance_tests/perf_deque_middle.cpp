#include "perf_common.h"
#include "perf_suites.h"

void run_deque_middle()
{
    MAKE_SUITE(DEQUE_MIDDLE_TEST, 10000, deque_middle);
}
