#include "perf_common.h"

#include <boost/property_tree/json_parser.hpp>

static_vector_impl(TestVector, int, STATIC_CONTAINER_SIZE);
static_list_impl(TestList, int, STATIC_CONTAINER_SIZE);
static_unordered_set_impl(TestHashTable, int, STATIC_CONTAINER_SIZE);
static_set_impl(TestBinaryTree, int, STATIC_CONTAINER_SIZE);
static_deque_impl(TestDeque, int, STATIC_CONTAINER_SIZE);
static_map_impl(TestMap, int, int, STATIC_CONTAINER_SIZE);
static_unordered_map_impl(TestUnorderedMap, int, int, STATIC_CONTAINER_SIZE);

dynamic_memory_impl(DynamicAllocator);
custom_allocator_vector_impl(DynamicAllocatorVector, int, DynamicAllocator);
custom_allocator_list_impl(DynamicAllocatorList, int, DynamicAllocator);
custom_allocator_deque_impl(DynamicAllocatorDeque, int, DynamicAllocator);
dynamic_deque_impl(DynamicDeque, int);
custom_allocator_unordered_set_impl(DynamicAllocatorHashTable, int, DynamicAllocator);
custom_allocator_set_impl(DynamicAllocatorBinaryTree, int, DynamicAllocator);
custom_allocator_map_impl(DynamicAllocatorMap, int, int, DynamicAllocator);
custom_allocator_unordered_map_impl(DynamicUnorderedMap, int, int, DynamicAllocator);

static_pool_impl(TestPool, int, 20);

void addRecordToTree(boost::property_tree::ptree & array, std::string container, unsigned int duration)
{
    boost::property_tree::ptree child;

    child.put("container", container);
    child.put("duration", duration);

    array.push_back(std::make_pair("", child));

    std::cout << container << ": " << duration << std::endl;
}

void addRecordToTree2(boost::property_tree::ptree & array, std::string container, unsigned int x, unsigned int y)
{
    boost::property_tree::ptree child;

    child.put("container", container);
    child.put("x", x);
    child.put("y", y);

    array.push_back(std::make_pair("", child));

    std::cout << container << "(" << x << "): " << y << std::endl;
}

void generateFile(const boost::property_tree::ptree & array, std::string filename)
{
    boost::property_tree::ptree pt;

    pt.add_child("data", array);

    boost::property_tree::write_json(filename, pt);
}
