#include "perf_common.h"
#include "perf_suites.h"

static void runQueueTests()
{
    run_vec_back();
    run_list_back();
    run_list_front();
    run_list_middle();
    run_deque_back();
    run_deque_front();
    run_deque_middle();
}

static void runSetTests()
{
    run_set_find();
    run_unordered_set_find();
}

static void runMapTests()
{
    run_map_find();
    run_unordered_map_find();
}

int main()
{
    runQueueTests();
    runSetTests();
    runMapTests();
    return 0;
}
