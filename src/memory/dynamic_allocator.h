#pragma once

#define declare_dynamic_allocator_t(container_t, member_t) \
\
typedef struct container_t \
{ \
} container_t; \
\
void container_t##_Construct(container_t* const self); \
void container_t##_Destroy(container_t* const self); \
member_t* container_t##_Allocate(container_t* const self, size_t size); \
member_t* container_t##_Reallocate(container_t* const self, member_t* object, size_t new_size); \
void container_t##_Deallocate(container_t* const self, member_t* object);


#define define_dynamic_allocator_t(container_t, member_t) \
void container_t##_Construct(container_t* const self) \
{ \
    assert(self); \
} \
\
void container_t##_Destroy(container_t* const self) \
{ \
    assert(self); \
} \
\
member_t* container_t##_Allocate(container_t* const self, size_t size) \
{ \
    assert(self); \
    \
    return (member_t*)malloc(size * sizeof(member_t)); \
} \
\
member_t* container_t##_Reallocate(container_t* const self, member_t* object, size_t new_size) \
{ \
    assert(self); \
    \
    return (member_t*)realloc(object, new_size * sizeof(member_t)); \
} \
\
void container_t##_Deallocate(container_t* const self, member_t* object) \
{ \
    assert(self); \
    \
    free(object); \
} \

