#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "rtlib/error_codes.h"
#include "rtlib/list.h"

#define event_loop_t(container_t, member_t)                                                                   \
    typedef struct container_t container_t;                                                                   \
    typedef void (*container_t##_Function)(member_t);                                                         \
    typedef struct container_t##_##Member                                                                     \
    {                                                                                                         \
        container_t##_Function callback;                                                                      \
        member_t arg;                                                                                         \
    } container_t##_##Member;                                                                                 \
                                                                                                              \
    void container_t##_Construct(container_t * const self);                                                   \
    void container_t##_Destroy(container_t * const self);                                                     \
    size_t container_t##_Size(const container_t * const self);                                                \
    int container_t##_Enqueue(const container_t * const self, container_t##_Function callback, member_t arg); \
    int container_t##_Process(const container_t * const self);

#define static_event_loop_t(container_t, member_t, container_capacity)                                       \
    list_t(container_t##_List, container_t##_##Member);                                                      \
    static_list_t(container_t##_List, container_t##_##Member, container_capacity);                           \
                                                                                                             \
    struct container_t                                                                                       \
    {                                                                                                        \
        container_t##_List queue;                                                                            \
    };                                                                                                       \
                                                                                                             \
    void container_t##_Construct(container_t * const self)                                                   \
    {                                                                                                        \
        container_t##_List_Construct((container_t##_List *)&self->queue, NULL);                              \
    }                                                                                                        \
                                                                                                             \
    void container_t##_Destroy(container_t * const self)                                                     \
    {                                                                                                        \
        container_t##_List_Destroy((container_t##_List *)&self->queue);                                      \
    }                                                                                                        \
                                                                                                             \
    size_t container_t##_Size(const container_t * const self)                                                \
    {                                                                                                        \
        return container_t##_List_Size((container_t##_List *)&self->queue);                                  \
    }                                                                                                        \
                                                                                                             \
    int container_t##_Enqueue(const container_t * const self, container_t##_Function callback, member_t arg) \
    {                                                                                                        \
        container_t##_##Member member = { .callback = callback, .arg = arg };                                \
        return container_t##_List_PushBack((container_t##_List *)&self->queue, member);                      \
    }                                                                                                        \
                                                                                                             \
    int container_t##_Process(const container_t * const self)                                                \
    {                                                                                                        \
        if(!container_t##_List_Empty(&self->queue))                                                          \
        {                                                                                                    \
            container_t##_##Member event = container_t##_List_Front((container_t##_List *)&self->queue);     \
            container_t##_List_PopFront((container_t##_List *)&self->queue);                                 \
            event.callback(event.arg);                                                                       \
            return 1;                                                                                        \
        }                                                                                                    \
        return 0;                                                                                            \
    }

#define custom_allocator_event_loop_t(container_t, member_t, allocator_t)                                    \
    list_t(container_t##_List, container_t##_##Member);                                                      \
    custom_allocator_list_t(container_t##_List, container_t##_##Member, allocator_t);                        \
                                                                                                             \
    struct container_t                                                                                       \
    {                                                                                                        \
        container_t##_List queue;                                                                            \
    };                                                                                                       \
                                                                                                             \
    void container_t##_Construct(container_t * const self)                                                   \
    {                                                                                                        \
        container_t##_List_Construct((container_t##_List *)&self->queue, NULL);                              \
    }                                                                                                        \
                                                                                                             \
    void container_t##_Destroy(container_t * const self)                                                     \
    {                                                                                                        \
        container_t##_List_Destroy((container_t##_List *)&self->queue);                                      \
    }                                                                                                        \
                                                                                                             \
    size_t container_t##_Size(const container_t * const self)                                                \
    {                                                                                                        \
        return container_t##_List_Size((container_t##_List *)&self->queue);                                  \
    }                                                                                                        \
                                                                                                             \
    int container_t##_Enqueue(const container_t * const self, container_t##_Function callback, member_t arg) \
    {                                                                                                        \
        container_t##_##Member member = { .callback = callback, .arg = arg };                                \
        return container_t##_List_PushBack((container_t##_List *)&self->queue, member);                      \
    }                                                                                                        \
                                                                                                             \
    int container_t##_Process(const container_t * const self)                                                \
    {                                                                                                        \
        if(!container_t##_List_Empty(&self->queue))                                                          \
        {                                                                                                    \
            container_t##_##Member event = container_t##_List_Front((container_t##_List *)&self->queue);     \
            container_t##_List_PopFront((container_t##_List *)&self->queue);                                 \
            event.callback(event.arg);                                                                       \
            return 1;                                                                                        \
        }                                                                                                    \
        return 0;                                                                                            \
    }

#define dynamic_event_loop_t(container_t, member_t)                                                          \
    list_t(container_t##_List, container_t##_##Member);                                                      \
    dynamic_list_t(container_t##_List, container_t##_##Member);                                              \
                                                                                                             \
    struct container_t                                                                                       \
    {                                                                                                        \
        container_t##_List queue;                                                                            \
    };                                                                                                       \
                                                                                                             \
    void container_t##_Construct(container_t * const self)                                                   \
    {                                                                                                        \
        container_t##_List_Construct((container_t##_List *)&self->queue, NULL);                              \
    }                                                                                                        \
                                                                                                             \
    void container_t##_Destroy(container_t * const self)                                                     \
    {                                                                                                        \
        container_t##_List_Destroy((container_t##_List *)&self->queue);                                      \
    }                                                                                                        \
                                                                                                             \
    size_t container_t##_Size(const container_t * const self)                                                \
    {                                                                                                        \
        return container_t##_List_Size((container_t##_List *)&self->queue);                                  \
    }                                                                                                        \
                                                                                                             \
    int container_t##_Enqueue(const container_t * const self, container_t##_Function callback, member_t arg) \
    {                                                                                                        \
        container_t##_##Member member = { .callback = callback, .arg = arg };                                \
        return container_t##_List_PushBack((container_t##_List *)&self->queue, member);                      \
    }                                                                                                        \
                                                                                                             \
    int container_t##_Process(const container_t * const self)                                                \
    {                                                                                                        \
        if(!container_t##_List_Empty(&self->queue))                                                          \
        {                                                                                                    \
            container_t##_##Member event = container_t##_List_Front((container_t##_List *)&self->queue);     \
            container_t##_List_PopFront((container_t##_List *)&self->queue);                                 \
            event.callback(event.arg);                                                                       \
            return 1;                                                                                        \
        }                                                                                                    \
        return 0;                                                                                            \
    }
