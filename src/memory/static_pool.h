#include <assert.h>

#define declare_static_pool_t(type_t, member_t, pool_capacity)     \
                                                                   \
    typedef struct type_t##_node type_t##_node;                    \
                                                                   \
    typedef struct type_t##_node                                   \
    {                                                              \
        member_t value;                                            \
        type_t##_node * next;                                      \
    } type_t##_node;                                               \
                                                                   \
    typedef struct type_t                                          \
    {                                                              \
        type_t##_node nodes[pool_capacity];                        \
        type_t##_node * first_free_block;                          \
        type_t##_node * last_free_block;                           \
    } type_t;                                                      \
                                                                   \
    void type_t##_Construct(type_t * const self);                  \
    void type_t##_Destroy(type_t * const self);                    \
    member_t * type_t##_Allocate(type_t * const self);             \
    void type_t##_Release(type_t * const self, member_t * object); \
                                                                   \
    /* Additional method for static pool */                        \
    size_t type_t##_Capacity(type_t * const self);

#define define_static_pool_t(type_t, member_t, pool_capacity)               \
    void type_t##_Construct(type_t * const self)                            \
    {                                                                       \
        assert(self);                                                       \
                                                                            \
        self->first_free_block = self->nodes;                               \
                                                                            \
        for(int block_no = 0; block_no < pool_capacity - 1; ++block_no)     \
        {                                                                   \
            self->nodes[block_no].next = &self->nodes[block_no + 1];        \
        }                                                                   \
        self->last_free_block       = &self->nodes[pool_capacity - 1];      \
        self->last_free_block->next = NULL;                                 \
    }                                                                       \
                                                                            \
    void type_t##_Destroy(type_t * const self) { assert(self); }            \
                                                                            \
    member_t * type_t##_Allocate(type_t * const self)                       \
    {                                                                       \
        assert(self);                                                       \
                                                                            \
        if(self->first_free_block)                                          \
        {                                                                   \
            type_t##_node * to_return_block = self->first_free_block;       \
            self->first_free_block          = self->first_free_block->next; \
            return &to_return_block->value;                                 \
        }                                                                   \
        else                                                                \
        {                                                                   \
            return NULL;                                                    \
        }                                                                   \
    }                                                                       \
                                                                            \
    void type_t##_Release(type_t * const self, member_t * object)           \
    {                                                                       \
        assert(self);                                                       \
        assert(object);                                                     \
                                                                            \
        type_t##_node * node        = (type_t##_node *)object;              \
        node->next                  = NULL;                                 \
        self->last_free_block->next = node;                                 \
        self->last_free_block       = node;                                 \
                                                                            \
        if(!self->first_free_block)                                         \
        {                                                                   \
            self->first_free_block = node;                                  \
        }                                                                   \
    }                                                                       \
                                                                            \
    size_t type_t##_Capacity(type_t * const self)                           \
    {                                                                       \
        assert(self);                                                       \
                                                                            \
        return pool_capacity;                                               \
    }
