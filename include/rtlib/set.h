#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define __set_methods_h(container_t, member_t)                                                                      \
    void container_t##_Construct(container_t * const self);                                                         \
    void container_t##_Destruct(container_t * const self);                                                          \
                                                                                                                    \
    size_t container_t##_Size(const container_t * const self);                                                      \
    bool container_t##_Empty(const container_t * const self);                                                       \
    int container_t##_Insert(container_t * const self, member_t data);                                              \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator);                     \
    void container_t##_Clear(container_t * const self);                                                             \
                                                                                                                    \
    container_t##_Iterator container_t##_Find(container_t * const self, member_t data);                             \
                                                                                                                    \
    container_t##_Iterator container_t##_Begin(const container_t * const self);                                     \
    container_t##_Iterator container_t##_End(const container_t * const self);                                       \
    const member_t * container_t##_CRef(const container_t * const self, const container_t##_Iterator * const self); \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                                   \
                                      const container_t##_Iterator * const second);                                 \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self);                                     \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self);

#ifdef __cplusplus
}
#endif