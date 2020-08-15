set(LIB_SOURCES
    containers/list.c
    containers/vector.c
    memory/heap.c
    memory/object_pool.c)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Werror -O3 -fno-exceptions")
set(CMAKE_C_FLAGS "-Wall -Werror -O3")

add_library(rtlib ${LIB_SOURCES})