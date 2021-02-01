#include <assert.h>

#define declare_custom_allocator_pool_t(pool_t, member_t, allocator_t) \
                                                                       \
    typedef struct pool_t                                              \
    {                                                                  \
        allocator_t allocator;                                         \
    } pool_t;                                                          \
                                                                       \
    void pool_t##_Construct(pool_t * const self);                      \
    void pool_t##_Destroy(pool_t * const self);                        \
    member_t * pool_t##_Allocate(pool_t * const self);                 \
    void pool_t##_Release(pool_t * const self, member_t * object);

#define define_custom_allocator_pool_t(pool_t, member_t, allocator_t)                  \
    void pool_t##_Construct(pool_t * const self)                                       \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        allocator_t##_Construct(&self->allocator);                                     \
    }                                                                                  \
                                                                                       \
    void pool_t##_Destroy(pool_t * const self)                                         \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        allocator_t##_Destroy(&self->allocator);                                       \
    }                                                                                  \
                                                                                       \
    member_t * pool_t##_Allocate(pool_t * const self)                                  \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        return (member_t *)allocator_t##_Allocate(&self->allocator, sizeof(member_t)); \
    }                                                                                  \
                                                                                       \
    void pool_t##_Release(pool_t * const self, member_t * object)                      \
    {                                                                                  \
        assert(self);                                                                  \
                                                                                       \
        allocator_t##_Deallocate(&self->allocator, object);                            \
    }
