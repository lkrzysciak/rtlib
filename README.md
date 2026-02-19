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

## Supported allocation models

Most containers are generated with macros and can be instantiated in one of these modes:

- static (fixed capacity, no runtime allocation)
- custom allocator (container uses your allocator type)
- dynamic (predefined allocator adapter based on malloc/realloc/free)

General naming pattern:

- declaration macro: `static_<container>`, `custom_allocator_<container>`, `dynamic_<container>`
- implementation macro: `static_<container>_impl`, `custom_allocator_<container>_impl`, `dynamic_<container>_impl`

## Comparator and hash requirements

- Ordered containers and search operations use `type_Compare(const type*, const type*)`.
- Unordered containers additionally need `type_Hash(const type*)`.
- For scalars, you can use helper macros from comparator/hash utilities (for example `comparator(int);` and `hash(int);`).

### User-defined types (recommended pattern)

For user-defined structs, prefer introducing a `typedef` name and use that name in RTLib macros.
This keeps generated symbol names (`type_Compare`, `type_Hash`) simple and avoids preprocessor edge cases.

```c
typedef struct MyType
{
   int id;
   int value;
} MyType;

comparator(MyType);
hash(MyType);

static_set(MySet, MyType, 128);
static_set_impl(MySet, MyType, 128);

static_unordered_set(MyUSet, MyType, 128);
static_unordered_set_impl(MyUSet, MyType, 128);
```

## Container quick start

### Vector

Operations: push/pop front/back, insert/erase, random access, find.

```c
#include "rtlib/vector.h"
#include "rtlib/comparator.h"

comparator(int);

static_vector(MyVec, int, 128);
static_vector_impl(MyVec, int, 128);

MyVec v;
MyVec_Construct(&v);
MyVec_PushBack(&v, 10);
MyVec_PushBack(&v, 20);
MyVec_Destruct(&v);
```

### List

Operations: push/pop front/back, insert/erase by iterator, sequential traversal.

```c
#include "rtlib/list.h"
#include "rtlib/comparator.h"

comparator(int);

static_list(MyList, int, 128);
static_list_impl(MyList, int, 128);

MyList l;
MyList_Construct(&l);
MyList_PushFront(&l, 1);
MyList_PushBack(&l, 2);
MyList_Destruct(&l);
```

### Deque

Operations: push/pop front/back, middle insert/erase, random access.

```c
#include "rtlib/deque.h"
#include "rtlib/comparator.h"

comparator(int);

static_deque(MyDeque, int, 256);
static_deque_impl(MyDeque, int, 256);

MyDeque d;
MyDeque_Construct(&d);
MyDeque_PushFront(&d, 7);
MyDeque_PushBack(&d, 9);
MyDeque_Destruct(&d);
```

### Set / Unordered set

`set`: ordered unique keys.  
`unordered_set`: hashed unique keys.

```c
#include "rtlib/set.h"
#include "rtlib/unordered_set.h"
#include "rtlib/comparator.h"
#include "rtlib/hash.h"

comparator(int);
hash(int);

static_set(MySet, int, 256);
static_set_impl(MySet, int, 256);

static_unordered_set(MyUSet, int, 256);
static_unordered_set_impl(MyUSet, int, 256);

MySet s;
MySet_Construct(&s);
MySet_Insert(&s, 42);
MySet_Destruct(&s);
```

### Map / Unordered map

`map`: ordered key-value pairs.  
`unordered_map`: hashed key-value pairs.

```c
#include "rtlib/map.h"
#include "rtlib/unordered_map.h"
#include "rtlib/comparator.h"
#include "rtlib/hash.h"

comparator(int);
hash(int);

static_map(MyMap, int, int, 256);
static_map_impl(MyMap, int, int, 256);

static_unordered_map(MyUMap, int, int, 256);
static_unordered_map_impl(MyUMap, int, int, 256);

MyMap m;
MyMap_Construct(&m);
MyMap_Insert(&m, 1, 100);
MyMap_Destruct(&m);
```

### String

RTLib string container supports push/insert/erase, compare, hash and find operations.

```c
#include "rtlib/string.h"

static_string(MyStr, 128);
static_string_impl(MyStr, 128);

MyStr s;
MyStr_Construct(&s);
MyStr_PushBack(&s, 'A');
MyStr_PushBack(&s, '\0');
MyStr_Destruct(&s);
```

## Memory utilities

### Pool allocator

- static pool: fixed number of preallocated objects
- custom allocator pool: object allocation delegated to allocator
- dynamic pool: helper based on dynamic allocator adapter

Headers: `rtlib/pool.h`

### Heap allocator

Headers: `rtlib/heap.h`

- `static_heap(type, capacity)` – in-object fixed buffer managed by split/coalesce allocator
- `dynamic_heap(type, capacity)` – one-time preallocated buffer (`malloc` in construct), then internal split/coalesce management

Both expose:

- `Allocate`
- `Callocate`
- `Reallocate`
- `Deallocate`

## Build and test

```bash
cmake -S . -B build
cmake --build build -j
./build/ci/ut
```

Run selected UT group:

```bash
./build/ci/ut --gtest_filter='*Heap*'
```

## Performance tests

Run all performance suites:

```bash
./build/ci/performance_tests_Os
```

Run selected suite(s):

```bash
./build/ci/performance_tests_Os --suite=queue
./build/ci/performance_tests_Os --suite=map
./build/ci/performance_tests_Os --suite=heap
```

JSON reports are generated in repository root (for example `deque_back_1000.json`), and GitHub workflow converts them to charts.


