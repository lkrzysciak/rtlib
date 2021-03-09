#pragma once

#include "../memory/dynamic_allocator.h"
#include "custom_allocator_list.h"

#define declare_dynamic_list_t(container_t, member_t)            \
    declare_dynamic_allocator_t(container_t##_DynamicAllocator); \
    declare_custom_allocator_list_t(container_t, member_t, container_t##_DynamicAllocator);

#define define_dynamic_list_t(container_t, member_t)            \
    define_dynamic_allocator_t(container_t##_DynamicAllocator); \
    define_custom_allocator_list_t(container_t, member_t, container_t##_DynamicAllocator);
