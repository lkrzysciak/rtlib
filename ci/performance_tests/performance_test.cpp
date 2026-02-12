#include "perf_common.h"
#include "perf_suites.h"

#include <string>
#include <unordered_set>
#include <vector>

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

static void splitSuites(const std::string & input, std::vector<std::string> & out)
{
    size_t start = 0;
    while(start < input.size())
    {
        size_t end = input.find(',', start);
        if(end == std::string::npos)
        {
            end = input.size();
        }
        if(end > start)
        {
            out.push_back(input.substr(start, end - start));
        }
        start = end + 1;
    }
}

static void printUsage(const char * name)
{
    std::cout << "Usage: " << name << " [--suite=<name>[,<name>...]]\n";
    std::cout << "Suites: all, queue, set, unordered_set, map, unordered_map\n";
}

int main(int argc, char ** argv)
{
    std::unordered_set<std::string> suites;
    for(int i = 1; i < argc; ++i)
    {
        std::string arg(argv[i]);
        if(arg == "--help" || arg == "-h")
        {
            printUsage(argv[0]);
            return 0;
        }

        const std::string prefix("--suite=");
        if(arg.rfind(prefix, 0) == 0)
        {
            std::vector<std::string> parsed;
            splitSuites(arg.substr(prefix.size()), parsed);
            for(const auto & suite : parsed)
            {
                suites.insert(suite);
            }
        }
        else
        {
            suites.insert(arg);
        }
    }

    if(suites.empty() || suites.count("all") > 0)
    {
        runQueueTests();
        runSetTests();
        runMapTests();
        return 0;
    }

    if(suites.count("queue") > 0)
    {
        runQueueTests();
    }
    if(suites.count("set") > 0)
    {
        run_set_find();
    }
    if(suites.count("unordered_set") > 0)
    {
        run_unordered_set_find();
    }
    if(suites.count("map") > 0)
    {
        run_map_find();
    }
    if(suites.count("unordered_map") > 0)
    {
        run_unordered_map_find();
    }
    return 0;
}
