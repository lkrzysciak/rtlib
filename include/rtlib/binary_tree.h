#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "rtlib/pool.h"
#include "error_codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define __binary_tree_methods_h(container_t, key_t)                                             \
    void container_t##_Construct(container_t * const self, container_t##_compare_t compare);    \
    void container_t##_Destroy(container_t * const self);                                       \
    size_t container_t##_Size(const container_t * const self);                                  \
    bool container_t##_Empty(const container_t * const self);                                   \
    int container_t##_Insert(container_t * const self, key_t data);                             \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator); \
    void container_t##_Clear(container_t * const self);                                         \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t data);            \
                                                                                                \
    container_t##_Iterator container_t##_Begin(const container_t * const self);                 \
    container_t##_Iterator container_t##_End(const container_t * const self);                   \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,               \
                                      const container_t##_Iterator * const second);             \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self);                 \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self);                 \
    const key_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self);       \
                                                                                                \
    /* will be deleted in v3*/                                                                  \
    key_t container_t##_Iterator_GetValue(const container_t##_Iterator * const self);           \
    void container_t##_Iterator_SetValue(container_t##_Iterator * const self, key_t key);       \
    container_t##_Iterator container_t##_CustomFind(container_t * const self, const key_t data, \
                                                    container_t##_compare_t compare_function);

#define __static_binary_tree_methods_c(container_t, key_t, container_capacity)                                         \
                                                                                                                       \
    static container_t##_node * __##container_t##_GetNextNode(container_t##_node * node)                               \
    {                                                                                                                  \
        if(!node)                                                                                                      \
        {                                                                                                              \
            return NULL;                                                                                               \
        }                                                                                                              \
        if(node->right != NULL)                                                                                        \
        {                                                                                                              \
            node = node->right;                                                                                        \
            while(node->left != NULL)                                                                                  \
            {                                                                                                          \
                node = node->left;                                                                                     \
            }                                                                                                          \
            return node;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        while(1)                                                                                                       \
        {                                                                                                              \
            if(node->parent == NULL)                                                                                   \
            {                                                                                                          \
                node = NULL;                                                                                           \
                return node;                                                                                           \
            }                                                                                                          \
            if(node->parent->left == node)                                                                             \
            {                                                                                                          \
                node = node->parent;                                                                                   \
                return node;                                                                                           \
            }                                                                                                          \
            node = node->parent;                                                                                       \
        }                                                                                                              \
        return node;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    static container_t##_node * __##container_t##_GetPrevNode(container_t##_node * node)                               \
    {                                                                                                                  \
        if(!node)                                                                                                      \
        {                                                                                                              \
            return NULL;                                                                                               \
        }                                                                                                              \
        if(node->left != NULL)                                                                                         \
        {                                                                                                              \
            node = node->left;                                                                                         \
            while(node->right != NULL)                                                                                 \
            {                                                                                                          \
                node = node->right;                                                                                    \
            }                                                                                                          \
            return node;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        while(1)                                                                                                       \
        {                                                                                                              \
            if(node->parent == NULL)                                                                                   \
            {                                                                                                          \
                node = NULL;                                                                                           \
                return node;                                                                                           \
            }                                                                                                          \
            if(node->parent->right == node)                                                                            \
            {                                                                                                          \
                node = node->parent;                                                                                   \
                return node;                                                                                           \
            }                                                                                                          \
            node = node->parent;                                                                                       \
        }                                                                                                              \
        return node;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Construct(container_t * const self, container_t##_compare_t compare)                            \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->size             = 0;                                                                                    \
        self->root             = NULL;                                                                                 \
        self->compare_function = compare;                                                                              \
        container_t##_pool_Construct(&self->pool);                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Destroy(container_t * const self)                                                               \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_pool_Destroy(&self->pool);                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    size_t container_t##_Size(const container_t * const self)                                                          \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Empty(const container_t * const self)                                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        return self->size == 0;                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Insert(container_t * const self, key_t data)                                                     \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_node * node = container_t##_pool_Allocate(&self->pool);                                          \
        if(node)                                                                                                       \
        {                                                                                                              \
            node->key = data;                                                                                          \
                                                                                                                       \
            if(self->root)                                                                                             \
            {                                                                                                          \
                container_t##_node * parent_node = NULL;                                                               \
                container_t##_node * child_node  = self->root;                                                         \
                                                                                                                       \
                while(1)                                                                                               \
                {                                                                                                      \
                    parent_node = child_node;                                                                          \
                                                                                                                       \
                    int compare_value = self->compare_function((const key_t *)&data, (const key_t *)&child_node->key); \
                    if(compare_value < 0)                                                                              \
                    {                                                                                                  \
                        child_node = child_node->left;                                                                 \
                        if(!child_node)                                                                                \
                        {                                                                                              \
                            parent_node->left = node;                                                                  \
                            break;                                                                                     \
                        }                                                                                              \
                    }                                                                                                  \
                    else if(compare_value > 0)                                                                         \
                    {                                                                                                  \
                        child_node = child_node->right;                                                                \
                        if(!child_node)                                                                                \
                        {                                                                                              \
                            parent_node->right = node;                                                                 \
                            break;                                                                                     \
                        }                                                                                              \
                    }                                                                                                  \
                    else                                                                                               \
                    {                                                                                                  \
                        container_t##_pool_Release(&self->pool, node);                                                 \
                        return ELEMENT_EXISTS;                                                                         \
                    }                                                                                                  \
                }                                                                                                      \
                node->right  = NULL;                                                                                   \
                node->left   = NULL;                                                                                   \
                node->parent = parent_node;                                                                            \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                node->right  = NULL;                                                                                   \
                node->left   = NULL;                                                                                   \
                node->parent = NULL;                                                                                   \
                self->root   = node;                                                                                   \
            }                                                                                                          \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            return ALLOCATION_ERROR;                                                                                   \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_node * to_delete_node            = iterator->node;                                               \
        container_t##_node * to_delete_node_parent     = iterator->node->parent;                                       \
        container_t##_node * new_node_at_this_Iterator = NULL;                                                         \
        assert(to_delete_node);                                                                                        \
                                                                                                                       \
        if(!to_delete_node->left && !to_delete_node->right)                                                            \
        {                                                                                                              \
            /* The node has no children */                                                                             \
            new_node_at_this_Iterator = NULL;                                                                          \
        }                                                                                                              \
        else if(to_delete_node->left && to_delete_node->right)                                                         \
        {                                                                                                              \
            /* The node has 2 children */                                                                              \
                                                                                                                       \
            /* Find minimum in right subtree */                                                                        \
            container_t##_node * minimum_node_in_right_subtree = to_delete_node->right;                                \
            while(minimum_node_in_right_subtree->left)                                                                 \
            {                                                                                                          \
                minimum_node_in_right_subtree = minimum_node_in_right_subtree->left;                                   \
            }                                                                                                          \
            /* Copy mininum key into to delete node */                                                                 \
            assert(minimum_node_in_right_subtree);                                                                     \
            to_delete_node->key = minimum_node_in_right_subtree->key;                                                  \
                                                                                                                       \
            /* Set to_delete_node pointer on new node */                                                               \
            to_delete_node        = minimum_node_in_right_subtree;                                                     \
            to_delete_node_parent = to_delete_node->parent;                                                            \
                                                                                                                       \
            /* Verify if to delete node has right node */                                                              \
            if(to_delete_node->right)                                                                                  \
            {                                                                                                          \
                new_node_at_this_Iterator = to_delete_node->right;                                                     \
            }                                                                                                          \
        }                                                                                                              \
        else if(to_delete_node->left)                                                                                  \
        {                                                                                                              \
            /* The node has only left child */                                                                         \
            new_node_at_this_Iterator    = to_delete_node->left;                                                       \
            to_delete_node->left->parent = to_delete_node_parent;                                                      \
        }                                                                                                              \
        else if(to_delete_node->right)                                                                                 \
        {                                                                                                              \
            /* The node has only right child */                                                                        \
            new_node_at_this_Iterator     = to_delete_node->right;                                                     \
            to_delete_node->right->parent = to_delete_node_parent;                                                     \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            assert(0);                                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        if(to_delete_node == self->root)                                                                               \
        {                                                                                                              \
            self->root = new_node_at_this_Iterator;                                                                    \
            if(self->root)                                                                                             \
            {                                                                                                          \
                self->root->parent = NULL;                                                                             \
            }                                                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            const bool is_to_delete_node_left = to_delete_node_parent->left == to_delete_node;                         \
                                                                                                                       \
            if(is_to_delete_node_left)                                                                                 \
            {                                                                                                          \
                to_delete_node_parent->left = new_node_at_this_Iterator;                                               \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                to_delete_node_parent->right = new_node_at_this_Iterator;                                              \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        container_t##_pool_Release(&self->pool, to_delete_node);                                                       \
                                                                                                                       \
        --self->size;                                                                                                  \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it = { 0 };                                                                             \
                                                                                                                       \
        /* Go to the last left node */                                                                                 \
        container_t##_node * parent_node = NULL;                                                                       \
        container_t##_node * child_node  = self->root;                                                                 \
        while(child_node)                                                                                              \
        {                                                                                                              \
            parent_node = child_node;                                                                                  \
            child_node  = child_node->left;                                                                            \
        }                                                                                                              \
        it.node = parent_node;                                                                                         \
        if(parent_node)                                                                                                \
        {                                                                                                              \
            if(parent_node->right)                                                                                     \
            {                                                                                                          \
                it.next = parent_node->right;                                                                          \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                it.next = parent_node->parent;                                                                         \
            }                                                                                                          \
        }                                                                                                              \
        it.prev = NULL;                                                                                                \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_End(const container_t * const self)                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it = { 0 };                                                                             \
                                                                                                                       \
        /* Go to the last right node */                                                                                \
        container_t##_node * parent_node = NULL;                                                                       \
        container_t##_node * child_node  = self->root;                                                                 \
        while(child_node)                                                                                              \
        {                                                                                                              \
            parent_node = child_node;                                                                                  \
            child_node  = child_node->right;                                                                           \
        }                                                                                                              \
        it.prev = parent_node;                                                                                         \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    key_t container_t##_Iterator_GetValue(const container_t##_Iterator * const self)                                   \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        key_t member = self->node->key;                                                                                \
        return member;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_SetValue(container_t##_Iterator * const self, key_t key)                               \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->node->key = key;                                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                                      \
                                      const container_t##_Iterator * const second)                                     \
    {                                                                                                                  \
        return first->node == second->node;                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->prev = self->node;                                                                                       \
        self->node = self->next;                                                                                       \
        if(!self->next)                                                                                                \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            self->next = __##container_t##_GetNextNode(self->node);                                                    \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->next = self->node;                                                                                       \
        self->node = self->prev;                                                                                       \
        if(!self->prev)                                                                                                \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            self->prev = __##container_t##_GetPrevNode(self->node);                                                    \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    const key_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                               \
    {                                                                                                                  \
        return (const key_t *)&self->node->key;                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t data)                                    \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it = { 0 };                                                                             \
                                                                                                                       \
        container_t##_node * node = self->root;                                                                        \
                                                                                                                       \
        while(node)                                                                                                    \
        {                                                                                                              \
            int compare_result = self->compare_function((const key_t *)&data, (const key_t *)&node->key);              \
            if(compare_result == 0)                                                                                    \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
            else if(compare_result < 0)                                                                                \
            {                                                                                                          \
                node = node->left;                                                                                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                node = node->right;                                                                                    \
            }                                                                                                          \
        }                                                                                                              \
        it.node = node;                                                                                                \
        it.next = __##container_t##_GetNextNode(node);                                                                 \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_CustomFind(container_t * const self, const key_t data,                        \
                                                    container_t##_compare_t compare_function)                          \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(compare_function);                                                                                      \
                                                                                                                       \
        container_t##_Iterator end = container_t##_End(self);                                                          \
        container_t##_Iterator it  = container_t##_Begin(self);                                                        \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                         \
                                                                                                                       \
        {                                                                                                              \
            const key_t it_value = container_t##_Iterator_GetValue(&it);                                               \
            if(compare_function(&data, &it_value) == 0)                                                                \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Clear(container_t * const self)                                                                 \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        while(container_t##_Size(self) != 0)                                                                           \
        {                                                                                                              \
            container_t##_Iterator begin = container_t##_Begin(self);                                                  \
            container_t##_Erase(self, &begin);                                                                         \
        }                                                                                                              \
    }

#define __custom_allocator_binary_tree_methods_c(container_t, key_t, allocator_t)                                      \
                                                                                                                       \
    static container_t##_node * __##container_t##_GetNextNode(container_t##_node * node)                               \
    {                                                                                                                  \
        if(!node)                                                                                                      \
        {                                                                                                              \
            return NULL;                                                                                               \
        }                                                                                                              \
        if(node->right != NULL)                                                                                        \
        {                                                                                                              \
            node = node->right;                                                                                        \
            while(node->left != NULL)                                                                                  \
            {                                                                                                          \
                node = node->left;                                                                                     \
            }                                                                                                          \
            return node;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        while(1)                                                                                                       \
        {                                                                                                              \
            if(node->parent == NULL)                                                                                   \
            {                                                                                                          \
                node = NULL;                                                                                           \
                return node;                                                                                           \
            }                                                                                                          \
            if(node->parent->left == node)                                                                             \
            {                                                                                                          \
                node = node->parent;                                                                                   \
                return node;                                                                                           \
            }                                                                                                          \
            node = node->parent;                                                                                       \
        }                                                                                                              \
        return node;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    static container_t##_node * __##container_t##_GetPrevNode(container_t##_node * node)                               \
    {                                                                                                                  \
        if(!node)                                                                                                      \
        {                                                                                                              \
            return NULL;                                                                                               \
        }                                                                                                              \
        if(node->left != NULL)                                                                                         \
        {                                                                                                              \
            node = node->left;                                                                                         \
            while(node->right != NULL)                                                                                 \
            {                                                                                                          \
                node = node->right;                                                                                    \
            }                                                                                                          \
            return node;                                                                                               \
        }                                                                                                              \
                                                                                                                       \
        while(1)                                                                                                       \
        {                                                                                                              \
            if(node->parent == NULL)                                                                                   \
            {                                                                                                          \
                node = NULL;                                                                                           \
                return node;                                                                                           \
            }                                                                                                          \
            if(node->parent->right == node)                                                                            \
            {                                                                                                          \
                node = node->parent;                                                                                   \
                return node;                                                                                           \
            }                                                                                                          \
            node = node->parent;                                                                                       \
        }                                                                                                              \
        return node;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Construct(container_t * const self, container_t##_compare_t compare)                            \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->size             = 0;                                                                                    \
        self->root             = NULL;                                                                                 \
        self->compare_function = compare;                                                                              \
        allocator_t##_Construct(&self->allocator);                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Destroy(container_t * const self)                                                               \
    {                                                                                                                  \
        while(container_t##_Size(self) != 0)                                                                           \
        {                                                                                                              \
            container_t##_Iterator begin = container_t##_Begin(self);                                                  \
            container_t##_Erase(self, &begin);                                                                         \
        }                                                                                                              \
        allocator_t##_Destroy(&self->allocator);                                                                       \
    }                                                                                                                  \
                                                                                                                       \
    size_t container_t##_Size(const container_t * const self)                                                          \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Empty(const container_t * const self)                                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        return self->size == 0;                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Insert(container_t * const self, key_t data)                                                     \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_node * node =                                                                                    \
            (container_t##_node *)allocator_t##_Allocate(&self->allocator, sizeof(container_t##_node));                \
        if(node)                                                                                                       \
        {                                                                                                              \
            node->key = data;                                                                                          \
                                                                                                                       \
            if(self->root)                                                                                             \
            {                                                                                                          \
                container_t##_node * parent_node = NULL;                                                               \
                container_t##_node * child_node  = self->root;                                                         \
                                                                                                                       \
                while(1)                                                                                               \
                {                                                                                                      \
                    parent_node = child_node;                                                                          \
                                                                                                                       \
                    int compare_value = self->compare_function((const key_t *)&data, (const key_t *)&child_node->key); \
                    if(compare_value < 0)                                                                              \
                    {                                                                                                  \
                        child_node = child_node->left;                                                                 \
                        if(!child_node)                                                                                \
                        {                                                                                              \
                            parent_node->left = node;                                                                  \
                            break;                                                                                     \
                        }                                                                                              \
                    }                                                                                                  \
                    else if(compare_value > 0)                                                                         \
                    {                                                                                                  \
                        child_node = child_node->right;                                                                \
                        if(!child_node)                                                                                \
                        {                                                                                              \
                            parent_node->right = node;                                                                 \
                            break;                                                                                     \
                        }                                                                                              \
                    }                                                                                                  \
                    else                                                                                               \
                    {                                                                                                  \
                        allocator_t##_Deallocate(&self->allocator, node);                                              \
                        return ELEMENT_EXISTS;                                                                         \
                    }                                                                                                  \
                }                                                                                                      \
                node->right  = NULL;                                                                                   \
                node->left   = NULL;                                                                                   \
                node->parent = parent_node;                                                                            \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                node->right  = NULL;                                                                                   \
                node->left   = NULL;                                                                                   \
                node->parent = NULL;                                                                                   \
                self->root   = node;                                                                                   \
            }                                                                                                          \
            ++self->size;                                                                                              \
                                                                                                                       \
            return self->size;                                                                                         \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            return ALLOCATION_ERROR;                                                                                   \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_node * to_delete_node            = iterator->node;                                               \
        container_t##_node * to_delete_node_parent     = iterator->node->parent;                                       \
        container_t##_node * new_node_at_this_Iterator = NULL;                                                         \
        assert(to_delete_node);                                                                                        \
                                                                                                                       \
        if(!to_delete_node->left && !to_delete_node->right)                                                            \
        {                                                                                                              \
            /* The node has no children */                                                                             \
            new_node_at_this_Iterator = NULL;                                                                          \
        }                                                                                                              \
        else if(to_delete_node->left && to_delete_node->right)                                                         \
        {                                                                                                              \
            /* The node has 2 children */                                                                              \
                                                                                                                       \
            /* Find minimum in right subtree */                                                                        \
            container_t##_node * minimum_node_in_right_subtree = to_delete_node->right;                                \
            while(minimum_node_in_right_subtree->left)                                                                 \
            {                                                                                                          \
                minimum_node_in_right_subtree = minimum_node_in_right_subtree->left;                                   \
            }                                                                                                          \
            /* Copy mininum key into to delete node */                                                                 \
            assert(minimum_node_in_right_subtree);                                                                     \
            to_delete_node->key = minimum_node_in_right_subtree->key;                                                  \
                                                                                                                       \
            /* Set to_delete_node pointer on new node */                                                               \
            to_delete_node        = minimum_node_in_right_subtree;                                                     \
            to_delete_node_parent = to_delete_node->parent;                                                            \
                                                                                                                       \
            /* Verify if to delete node has right node */                                                              \
            if(to_delete_node->right)                                                                                  \
            {                                                                                                          \
                new_node_at_this_Iterator = to_delete_node->right;                                                     \
            }                                                                                                          \
        }                                                                                                              \
        else if(to_delete_node->left)                                                                                  \
        {                                                                                                              \
            /* The node has only left child */                                                                         \
            new_node_at_this_Iterator    = to_delete_node->left;                                                       \
            to_delete_node->left->parent = to_delete_node_parent;                                                      \
        }                                                                                                              \
        else if(to_delete_node->right)                                                                                 \
        {                                                                                                              \
            /* The node has only right child */                                                                        \
            new_node_at_this_Iterator     = to_delete_node->right;                                                     \
            to_delete_node->right->parent = to_delete_node_parent;                                                     \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            assert(0);                                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        if(to_delete_node == self->root)                                                                               \
        {                                                                                                              \
            self->root = new_node_at_this_Iterator;                                                                    \
            if(self->root)                                                                                             \
            {                                                                                                          \
                self->root->parent = NULL;                                                                             \
            }                                                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            const bool is_to_delete_node_left = to_delete_node_parent->left == to_delete_node;                         \
                                                                                                                       \
            if(is_to_delete_node_left)                                                                                 \
            {                                                                                                          \
                to_delete_node_parent->left = new_node_at_this_Iterator;                                               \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                to_delete_node_parent->right = new_node_at_this_Iterator;                                              \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        allocator_t##_Deallocate(&self->allocator, to_delete_node);                                                    \
                                                                                                                       \
        --self->size;                                                                                                  \
        return self->size;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it = { 0 };                                                                             \
                                                                                                                       \
        /* Go to the last left node */                                                                                 \
        container_t##_node * parent_node = NULL;                                                                       \
        container_t##_node * child_node  = self->root;                                                                 \
        while(child_node)                                                                                              \
        {                                                                                                              \
            parent_node = child_node;                                                                                  \
            child_node  = child_node->left;                                                                            \
        }                                                                                                              \
        it.node = parent_node;                                                                                         \
        if(parent_node)                                                                                                \
        {                                                                                                              \
            if(parent_node->right)                                                                                     \
            {                                                                                                          \
                it.next = parent_node->right;                                                                          \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                it.next = parent_node->parent;                                                                         \
            }                                                                                                          \
        }                                                                                                              \
        it.prev = NULL;                                                                                                \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_End(const container_t * const self)                                           \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it = { 0 };                                                                             \
                                                                                                                       \
        /* Go to the last right node */                                                                                \
        container_t##_node * parent_node = NULL;                                                                       \
        container_t##_node * child_node  = self->root;                                                                 \
        while(child_node)                                                                                              \
        {                                                                                                              \
            parent_node = child_node;                                                                                  \
            child_node  = child_node->right;                                                                           \
        }                                                                                                              \
        it.prev = parent_node;                                                                                         \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    key_t container_t##_Iterator_GetValue(const container_t##_Iterator * const self)                                   \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        key_t member = self->node->key;                                                                                \
        return member;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_SetValue(container_t##_Iterator * const self, key_t key)                               \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->node->key = key;                                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                                      \
                                      const container_t##_Iterator * const second)                                     \
    {                                                                                                                  \
        return first->node == second->node;                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->prev = self->node;                                                                                       \
        self->node = self->next;                                                                                       \
        if(!self->next)                                                                                                \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            self->next = __##container_t##_GetNextNode(self->node);                                                    \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                         \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        self->next = self->node;                                                                                       \
        self->node = self->prev;                                                                                       \
        if(!self->prev)                                                                                                \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            self->prev = __##container_t##_GetPrevNode(self->node);                                                    \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    const key_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                               \
    {                                                                                                                  \
        return (const key_t *)&self->node->key;                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t data)                                    \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        container_t##_Iterator it = { 0 };                                                                             \
                                                                                                                       \
        container_t##_node * node = self->root;                                                                        \
                                                                                                                       \
        while(node)                                                                                                    \
        {                                                                                                              \
            int compare_result = self->compare_function((const key_t *)&data, (const key_t *)&node->key);              \
            if(compare_result == 0)                                                                                    \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
            else if(compare_result < 0)                                                                                \
            {                                                                                                          \
                node = node->left;                                                                                     \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                node = node->right;                                                                                    \
            }                                                                                                          \
        }                                                                                                              \
        it.node = node;                                                                                                \
        it.next = __##container_t##_GetNextNode(node);                                                                 \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    container_t##_Iterator container_t##_CustomFind(container_t * const self, const key_t data,                        \
                                                    container_t##_compare_t compare_function)                          \
    {                                                                                                                  \
        assert(self);                                                                                                  \
        assert(compare_function);                                                                                      \
                                                                                                                       \
        container_t##_Iterator end = container_t##_End(self);                                                          \
        container_t##_Iterator it  = container_t##_Begin(self);                                                        \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                         \
                                                                                                                       \
        {                                                                                                              \
            const key_t it_value = container_t##_Iterator_GetValue(&it);                                               \
            if(compare_function(&data, &it_value) == 0)                                                                \
            {                                                                                                          \
                break;                                                                                                 \
            }                                                                                                          \
        }                                                                                                              \
        return it;                                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    void container_t##_Clear(container_t * const self)                                                                 \
    {                                                                                                                  \
        assert(self);                                                                                                  \
                                                                                                                       \
        while(container_t##_Size(self) != 0)                                                                           \
        {                                                                                                              \
            container_t##_Iterator begin = container_t##_Begin(self);                                                  \
            container_t##_Erase(self, &begin);                                                                         \
        }                                                                                                              \
    }

#define binary_tree_t(container_t, key_t)                                 \
    typedef struct container_t container_t;                               \
    typedef struct container_t##_Iterator container_t##_Iterator;         \
    typedef struct container_t##_node container_t##_node;                 \
    typedef int (*container_t##_compare_t)(const key_t *, const key_t *); \
                                                                          \
    __binary_tree_methods_h(container_t, key_t)

#define static_binary_tree_t(container_t, key_t, container_capacity)           \
    struct container_t##_node                                                  \
    {                                                                          \
        container_t##_node * right;                                            \
        container_t##_node * left;                                             \
        container_t##_node * parent;                                           \
        key_t key;                                                             \
    };                                                                         \
                                                                               \
    struct container_t##_Iterator                                              \
    {                                                                          \
        container_t##_node * node;                                             \
        container_t##_node * next;                                             \
        container_t##_node * prev;                                             \
    };                                                                         \
    pool_t(container_t##_pool, container_t##_node);                            \
    static_pool_t(container_t##_pool, container_t##_node, container_capacity); \
                                                                               \
    struct container_t                                                         \
    {                                                                          \
        container_t##_node * root;                                             \
        container_t##_pool pool;                                               \
        container_t##_compare_t compare_function;                              \
        size_t size;                                                           \
    };                                                                         \
    __static_binary_tree_methods_c(container_t, key_t, container_capacity)

#define custom_allocator_binary_tree_t(container_t, key_t, allocator_t) \
    struct container_t##_node                                           \
    {                                                                   \
        container_t##_node * right;                                     \
        container_t##_node * left;                                      \
        container_t##_node * parent;                                    \
        key_t key;                                                      \
    };                                                                  \
                                                                        \
    struct container_t##_Iterator                                       \
    {                                                                   \
        container_t##_node * node;                                      \
        container_t##_node * next;                                      \
        container_t##_node * prev;                                      \
    };                                                                  \
                                                                        \
    struct container_t                                                  \
    {                                                                   \
        container_t##_node * root;                                      \
        allocator_t allocator;                                          \
        container_t##_compare_t compare_function;                       \
        size_t size;                                                    \
    };                                                                  \
    __custom_allocator_binary_tree_methods_c(container_t, key_t, allocator_t)

#include "rtlib/memory.h"

#define dynamic_binary_tree_t(container_t, key_t)     \
    memory_t(container_t##_DynamicAllocator);         \
    dynamic_memory_t(container_t##_DynamicAllocator); \
    custom_allocator_binary_tree_t(container_t, key_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif
