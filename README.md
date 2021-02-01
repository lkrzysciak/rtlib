# rtlib

### Overview
The library for real-time systems. The developer has a full control of memory. Using dynamic memory allocation in most embedded systems is forbidden, due to not deterministic allocation time. The rtlib provides a static allocated memory modules as well as a custom allocator implemetations, which minimalize non deterministic operations. 

It contains:
 * containers
   * vector
   * list
   * binary tree
   * hash table
 * memory pools

The library has been written in C language in a pseudo template style (only includes are needed). It allows a compiler to use a copy-elision optimalization - see performance tests results. Most of rtlib containers are faster than STL equivalents.

### Why macros?
Most of C developers uses void* pointer to pass unknown size object to e.g custom list implementation. This approach has one huge disadvantage. A compiler doesn't know a type of object, which results in blocking copy-elision optimalization. Passing the objects by void* pointer can be faster if optimalization is disabled (O0 flag), otherwise macro approach is much faster.
