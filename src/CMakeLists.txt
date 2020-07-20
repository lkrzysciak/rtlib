set(LIB_SOURCES
    containers/list.c
    memory/heap.c
    memory/object_pool.c)

add_library(rtlib ${LIB_SOURCES})