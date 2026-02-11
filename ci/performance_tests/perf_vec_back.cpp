#include "perf_common.h"
#include "perf_suites.h"

void run_vec_back()
{
    MAKE_SUITE(VECTOR_BACK_TEST, 300000, vec_back);
}
