# RTLib

### Overview
RTLib is type-safe container library written in C language. Each component provides a few methods of memory allocation. It can use static
or dynamic allocated memory. You can write your own custom memory allocator as well. Static implemenations are generaly faster and has more 
deterministic behaviour (due to no heap fragmentation overhead). However, dynamic containers are more reusable.

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

The library has been written in C language as a C++ template style (only includes are needed). 


