#include <stdio.h>
#include "containers/static_vector.h"
#include "containers/static_list.h"
#include "containers/static_binary_tree.h"
#include "containers/static_hash_table.h"
#include "containers/custom_allocator_vector.h"
#include "containers/custom_allocator_list.h"
#include "containers/custom_allocator_binary_tree.h"
#include "containers/custom_allocator_hash_table.h"
#include "memory/dynamic_allocator.h"

declare_dynamic_allocator_t(DynamicAllocator);
define_dynamic_allocator_t(DynamicAllocator);

declare_static_vector_t(TestVector, int, 20);
define_static_vector_t(TestVector, int, 20);

declare_custom_allocator_vector_t(CustomVector, int, DynamicAllocator);
define_custom_allocator_vector_t(CustomVector, int, DynamicAllocator);

declare_static_list_t(TestList, int, 20);
define_static_list_t(TestList, int, 20);

declare_custom_allocator_list_t(CustomList, int, DynamicAllocator);
define_custom_allocator_list_t(CustomList, int, DynamicAllocator);

declare_static_binary_tree_t(TestBinaryTree, int, 20);
define_static_binary_tree_t(TestBinaryTree, int, 20);

declare_custom_allocator_binary_tree_t(CustomBinaryTree, int, DynamicAllocator);
define_custom_allocator_binary_tree_t(CustomBinaryTree, int, DynamicAllocator);

declare_static_hash_table_t(TestHashTable, int, 20);
define_static_hash_table_t(TestHashTable, int, 20);

declare_custom_allocator_hash_table_t(CustomHashTable, int, DynamicAllocator);
define_custom_allocator_hash_table_t(CustomHashTable, int, DynamicAllocator);

int main()
{
    TestVector test_vector;
    TestVector_Construct(&test_vector);
    TestVector_PushBack(&test_vector, 5);
    TestVector_Destroy(&test_vector);
    printf("%d\n", TestVector_Front(&test_vector));
    
    TestList test_list;
    TestList_Construct(&test_list);
    TestList_PushBack(&test_list, 5);
    TestList_Destroy(&test_list);
    printf("%d\n", TestList_Front(&test_list));

    return 0;
}