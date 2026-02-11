# RTLib

### Overview
RTLib is a type-safe container library written in C. Each component provides a few methods of memory allocation. It can use static
or dynamic allocated memory. You can write your own custom memory allocator as well. Static implementations are generally faster and have more
deterministic behavior (due to no heap fragmentation overhead). However, dynamic containers are more reusable.

### Features
Supported containers:
   * vector
   * list
   * deque
   * set
   * unordered set
   * map
   * unordered map
   * string

Memory utilities:
   * pool allocator (static/custom/dynamic)
   * heap allocator (static/dynamic with Allocate/Callocate/Reallocate/Deallocate)
   * dynamic memory adapter (malloc/realloc/free)

The library has been written in C language as a C++ template style (only includes are needed).


