#include <assert.h>

#define typed_pool_t(type_name, member_type, pool_capacity) \
\
typedef struct type_name##_node type_name##_node; \
\
typedef struct type_name##_node \
{ \
    member_type value; \
    type_name##_node* next; \
} type_name##_node; \
\
typedef struct type_name \
{ \
    type_name##_node nodes[pool_capacity]; \
    type_name##_node* first_free_block; \
    type_name##_node* last_free_block; \
} type_name; \
\
void type_name##_Init(type_name* const self) \
{ \
    assert(self); \
    \
    self->first_free_block = self->nodes; \
    \
    for(int block_no=0; block_no<pool_capacity-1; ++block_no) \
    { \
        self->nodes[block_no].next = &self->nodes[block_no + 1]; \
    } \
    self->last_free_block = &self->nodes[pool_capacity - 1]; \
    self->last_free_block->next = NULL; \
} \
\
member_type* type_name##_Alloc(type_name* const self) \
{ \
    assert(self); \
    \
    if(self->first_free_block) \
    { \
        type_name##_node* to_return_block = self->first_free_block; \
        self->first_free_block = self->first_free_block->next; \
        return &to_return_block->value; \
    } \
    else \
    { \
        return NULL; \
    } \
} \
\
void type_name##_Free(type_name* const self, member_type* object) \
{ \
    assert(self); \
    assert(object); \
    \
    type_name##_node* node = (type_name##_node*)object; \
    node->next = NULL; \
    self->last_free_block->next = node; \
    self->last_free_block = node; \
    \
    if(!self->first_free_block) \
    { \
        self->first_free_block = node; \
    } \
} \
