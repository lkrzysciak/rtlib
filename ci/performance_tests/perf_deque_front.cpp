#include "perf_common.h"
#include "perf_suites.h"

void run_deque_front()
{
    MAKE_SUITE(DEQUE_FRONT_TEST, 300000, deque_front);
}
