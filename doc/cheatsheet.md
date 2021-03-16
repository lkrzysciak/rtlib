# Rtlib cheat sheet
 
 The library provides similar to c++ template mechanism to create new types. However it doesn't not provide additional template processor, so you new created new types manually.

## Create static rtlib container

```
declare_static_<template_type>_t(<new_type_name>, <member_type>, <container_capacity>);
define_static_<template_type>_t(<new_type_name>, <member_type>, <container_capacity>);
```

Example:

```
declare_static_vector_t(VectorTestType, int, 5);
define_static_vector_t(VectorTestType, int, 5);
```

## Create custom allocator rtlib container

Each custom allocator container requires allocator definition and declaration. The rtlib provides wrapper for standard dynamic allocation operations. You can create it as:

```
declare_dynamic_allocator_t(<allocator_name>);
define_dynamic_allocator_t(<allocator_name>);
```

Example:
```
declare_dynamic_allocator_t(DynamicAllocator);
define_dynamic_allocator_t(DynamicAllocator);
```

You can also write your own allocator. 

Now define and declare new container type:
```
declare_custom_allocator_<template_type>_t(<new_type_name>, <member_type>, <allocator_type>);
define_custom_allocator_<template_type>_t(<new_type_name>, <member_type>, <allocator_type>);
```

Example:
```
declare_custom_allocator_vector_t(CustomAllocatorVector, int, DynamicAllocator);
define_custom_allocator_vector_t(CustomAllocatorVector, int, DynamicAllocator);
```

## Create dynamic rtlib container

```
declare_dynamic_<template_type>_t(<new_type_name>, <member_type>);
define_dynamic_<template_type>_t(<new_type_name>, <member_type>);
```

Example:

```
declare_dynamic_vector_t(VectorTestType, int);
define_dynamic_vector_t(VectorTestType, int);
```

## Typical work flow is:

1. Create new type
2. Construct new object (init object with default values or allocate memory if necesarry)
3. Use the containers (add, remove, modify, iterate etc.)
4. Destroy container (deallocate memory if necessary)

## Container interfaces

The rtlib keeps similar function names as C++ STL. For more information please look at simple tests which shows how to create and use containers (https://github.com/lukas9409/rtlib/tree/master/ci/c_tests).

For more information analyze UTs: (https://github.com/lukas9409/rtlib/tree/master/ci)

## Container replaceable

Vector and list are full replaceable as well as binary tree and hash table. 
