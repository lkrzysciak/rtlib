#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "rtlib/pool.h"
#include "error_codes.h"
#include "rtlib/memory.h"

#ifndef RED
#define RED 0
#endif

#ifndef BLACK
#define BLACK 1
#endif

#define __binary_tree_methods_h(container_t, key_t)                                             \
    void container_t##_Construct(container_t * const self);                                     \
    void container_t##_Destruct(container_t * const self);                                      \
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
    const key_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self);

#define __private_set_methods_c(container_t, key_t)                                                              \
    static container_t##_node * __##container_t##_Successor(container_t * const self, container_t##_node * node) \
    {                                                                                                            \
        container_t##_node * p;                                                                                  \
                                                                                                                 \
        p = node->right;                                                                                         \
                                                                                                                 \
        if(p != NULL)                                                                                            \
        {                                                                                                        \
            /* move down until we find it */                                                                     \
            for(; p->left != NULL; p = p->left)                                                                  \
                ;                                                                                                \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            /* move up until we find it or hit the root */                                                       \
            for(p = node->parent; node == p->right; node = p, p = p->parent)                                     \
                ;                                                                                                \
                                                                                                                 \
            if(p == self->root)                                                                                  \
                p = NULL; /* not found */                                                                        \
        }                                                                                                        \
                                                                                                                 \
        return p;                                                                                                \
    }                                                                                                            \
                                                                                                                 \
    static void __##container_t##_RotateLeft(container_t * const self, container_t##_node * x)                   \
    {                                                                                                            \
        container_t##_node * y;                                                                                  \
                                                                                                                 \
        y = x->right;                                                                                            \
                                                                                                                 \
        x->right = y->left;                                                                                      \
        if(x->right != NULL)                                                                                     \
            x->right->parent = x;                                                                                \
                                                                                                                 \
        y->parent = x->parent;                                                                                   \
        if(x->parent && x == x->parent->left)                                                                    \
        {                                                                                                        \
            x->parent->left = y;                                                                                 \
        }                                                                                                        \
        else if(x->parent && x == x->parent->right)                                                              \
        {                                                                                                        \
            x->parent->right = y;                                                                                \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            self->root = y;                                                                                      \
        }                                                                                                        \
        y->left   = x;                                                                                           \
        x->parent = y;                                                                                           \
    }                                                                                                            \
                                                                                                                 \
    static void __##container_t##_RotateRight(container_t * const self, container_t##_node * x)                  \
    {                                                                                                            \
        container_t##_node * y;                                                                                  \
                                                                                                                 \
        y = x->left;                                                                                             \
        printf("x: %p, y: %p\n", x, y);                                                                          \
        x->left = y->right;                                                                                      \
        if(x->left != NULL)                                                                                      \
            x->left->parent = x;                                                                                 \
                                                                                                                 \
        y->parent = x->parent;                                                                                   \
        if(x->parent && x == x->parent->left)                                                                    \
        {                                                                                                        \
            x->parent->left = y;                                                                                 \
        }                                                                                                        \
        else if(x->parent && x == x->parent->right)                                                              \
        {                                                                                                        \
            x->parent->right = y;                                                                                \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            self->root = y;                                                                                      \
        }                                                                                                        \
        y->right  = x;                                                                                           \
        x->parent = y;                                                                                           \
    }                                                                                                            \
                                                                                                                 \
    static void __##container_t##_BlackRedTreeBalance(container_t * const self, container_t##_node * current)    \
    {                                                                                                            \
        container_t##_node * uncle;                                                                              \
                                                                                                                 \
        do                                                                                                       \
        {                                                                                                        \
            /* current node is RED and parent node is RED */                                                     \
            if(current->parent == current->parent->parent->left)                                                 \
            {                                                                                                    \
                uncle = current->parent->parent->right;                                                          \
                if(uncle && uncle->color == RED)                                                                 \
                {                                                                                                \
                    /* insertion into 4-children cluster */                                                      \
                                                                                                                 \
                    /* split */                                                                                  \
                    current->parent->color = BLACK;                                                              \
                    uncle->color           = BLACK;                                                              \
                                                                                                                 \
                    /* send grandparent node up the tree */                                                      \
                    current        = current->parent->parent; /* goto loop or break */                           \
                    current->color = RED;                                                                        \
                }                                                                                                \
                else                                                                                             \
                {                                                                                                \
                    /* insertion into 3-children cluster */                                                      \
                                                                                                                 \
                    /* equivalent BST */                                                                         \
                    if(current == current->parent->right)                                                        \
                    {                                                                                            \
                        current = current->parent;                                                               \
                        __##container_t##_RotateLeft(self, current);                                             \
                    }                                                                                            \
                                                                                                                 \
                    /* 3-children cluster has two representations */                                             \
                    current->parent->color         = BLACK; /* thus goto break */                                \
                    current->parent->parent->color = RED;                                                        \
                    __##container_t##_RotateRight(self, current->parent->parent);                                \
                }                                                                                                \
            }                                                                                                    \
            else                                                                                                 \
            {                                                                                                    \
                uncle = current->parent->parent->left;                                                           \
                if(uncle && uncle->color == RED)                                                                 \
                {                                                                                                \
                    /* insertion into 4-children cluster */                                                      \
                                                                                                                 \
                    /* split */                                                                                  \
                    current->parent->color = BLACK;                                                              \
                    uncle->color           = BLACK;                                                              \
                                                                                                                 \
                    /* send grandparent node up the tree */                                                      \
                    current        = current->parent->parent; /* goto loop or break */                           \
                    current->color = RED;                                                                        \
                }                                                                                                \
                else                                                                                             \
                {                                                                                                \
                    /* insertion into 3-children cluster */                                                      \
                                                                                                                 \
                    /* equivalent BST */                                                                         \
                    if(current == current->parent->left)                                                         \
                    {                                                                                            \
                        current = current->parent;                                                               \
                        __##container_t##_RotateRight(self, current);                                            \
                    }                                                                                            \
                                                                                                                 \
                    /* 3-children cluster has two representations */                                             \
                    current->parent->color         = BLACK; /* thus goto break */                                \
                    current->parent->parent->color = RED;                                                        \
                    __##container_t##_RotateLeft(self, current->parent->parent);                                 \
                }                                                                                                \
            }                                                                                                    \
        } while(current->parent && current->parent->color == RED);                                               \
    }                                                                                                            \
                                                                                                                 \
    static void __##container_t##_BalanceErase(container_t * const self, container_t##_node * current)           \
    {                                                                                                            \
        container_t##_node * sibling;                                                                            \
        printf("Size: %d\n", self->size);                                                                        \
        do                                                                                                       \
        {                                                                                                        \
            printf("BalanceErase current: %p , parent: %p\n", current, current->parent);                         \
            if(current == current->parent->left)                                                                 \
            {                                                                                                    \
                sibling = current->parent->right;                                                                \
                printf("Is left - sbilling: %p\n", sibling);                                                     \
                                                                                                                 \
                if(sibling->color == RED)                                                                        \
                {                                                                                                \
                    /* perform an adjustment (3-children parent cluster has two representations) */              \
                    sibling->color         = BLACK;                                                              \
                    current->parent->color = RED;                                                                \
                    __##container_t##_RotateLeft(self, current->parent);                                         \
                    sibling = current->parent->right;                                                            \
                }                                                                                                \
                                                                                                                 \
                /* sibling node must be BLACK now */                                                             \
                                                                                                                 \
                if(sibling->right->color == BLACK && sibling->left->color == BLACK)                              \
                {                                                                                                \
                    /* 2-children sibling cluster, fuse by recoloring */                                         \
                    sibling->color = RED;                                                                        \
                    if(current->parent->color == RED)                                                            \
                    { /* 3/4-children parent cluster */                                                          \
                        current->parent->color = BLACK;                                                          \
                        break; /* goto break */                                                                  \
                    }                                                                                            \
                    else                                                                                         \
                    {                              /* 2-children parent cluster */                               \
                        current = current->parent; /* goto loop */                                               \
                    }                                                                                            \
                }                                                                                                \
                else                                                                                             \
                {                                                                                                \
                    /* 3/4-children sibling cluster */                                                           \
                                                                                                                 \
                    /* perform an adjustment (3-children sibling cluster has two representations) */             \
                    if(sibling->right->color == BLACK)                                                           \
                    {                                                                                            \
                        sibling->left->color = BLACK;                                                            \
                        sibling->color       = RED;                                                              \
                        __##container_t##_RotateRight(self, sibling);                                            \
                        sibling = current->parent->right;                                                        \
                    }                                                                                            \
                                                                                                                 \
                    /* transfer by rotation and recoloring */                                                    \
                    sibling->color         = current->parent->color;                                             \
                    current->parent->color = BLACK;                                                              \
                    sibling->right->color  = BLACK;                                                              \
                    __##container_t##_RotateLeft(self, current->parent);                                         \
                    break; /* goto break */                                                                      \
                }                                                                                                \
            }                                                                                                    \
            else if(current == current->parent->right)                                                           \
            {                                                                                                    \
                printf("Is right %p\n", current->parent->left);                                                  \
                sibling = current->parent->left;                                                                 \
                                                                                                                 \
                if(sibling && sibling->color == RED)                                                             \
                {                                                                                                \
                    printf("Sbilling is red\n");                                                                 \
                    /* perform an adjustment (3-children parent cluster has two representations) */              \
                    sibling->color         = BLACK;                                                              \
                    current->parent->color = RED;                                                                \
                    __##container_t##_RotateRight(self, current->parent);                                        \
                    sibling = current->parent->left;                                                             \
                }                                                                                                \
                                                                                                                 \
                /* sibling node must be BLACK now */                                                             \
                printf("Checks is childs are black\n");                                                          \
                if(sibling && sibling->right && sibling->right->color == BLACK && sibling->left &&               \
                   sibling->left->color == BLACK)                                                                \
                {                                                                                                \
                    printf("Are black\n");                                                                       \
                    /* 2-children sibling cluster, fuse by recoloring */                                         \
                    sibling->color = RED;                                                                        \
                    if(current->parent->color == RED)                                                            \
                    { /* 3/4-children parent cluster */                                                          \
                        current->parent->color = BLACK;                                                          \
                        break; /* goto break */                                                                  \
                    }                                                                                            \
                    else                                                                                         \
                    {                              /* 2-children parent cluster */                               \
                        current = current->parent; /* goto loop */                                               \
                    }                                                                                            \
                }                                                                                                \
                else if(sibling)                                                                                 \
                {                                                                                                \
                    printf("Not black\n");                                                                       \
                    /* 3/4-children sibling cluster */                                                           \
                                                                                                                 \
                    /* perform an adjustment (3-children sibling cluster has two representations) */             \
                    if(sibling && sibling->left && sibling->left->color == BLACK)                                \
                    {                                                                                            \
                        if(sibling->right)                                                                       \
                            sibling->right->color = BLACK;                                                       \
                        sibling->color = RED;                                                                    \
                        printf("before rotate\n");                                                               \
                        __##container_t##_RotateLeft(self, sibling);                                             \
                        printf("after rotate\n");                                                                \
                        sibling = current->parent->left;                                                         \
                    }                                                                                            \
                                                                                                                 \
                    /* transfer by rotation and recoloring */                                                    \
                    printf("1\n");                                                                               \
                    if(sibling)                                                                                  \
                        sibling->color = current->parent->color;                                                 \
                    printf("2\n");                                                                               \
                    current->parent->color = BLACK;                                                              \
                    if(sibling && sibling->left)                                                                 \
                        sibling->left->color = BLACK;                                                            \
                    printf("3\n");                                                                               \
                    __##container_t##_RotateRight(self, current->parent);                                        \
                    printf("4\n");                                                                               \
                    break; /* goto break */                                                                      \
                }                                                                                                \
                else                                                                                             \
                {                                                                                                \
                    break;                                                                                       \
                }                                                                                                \
            }                                                                                                    \
        } while(current != self->root);                                                                          \
    }                                                                                                            \
                                                                                                                 \
    static container_t##_node * __##container_t##_GetNextNode(container_t##_node * node)                         \
    {                                                                                                            \
        if(!node)                                                                                                \
        {                                                                                                        \
            return NULL;                                                                                         \
        }                                                                                                        \
        if(node->right != NULL)                                                                                  \
        {                                                                                                        \
            node = node->right;                                                                                  \
            while(node->left != NULL)                                                                            \
            {                                                                                                    \
                node = node->left;                                                                               \
            }                                                                                                    \
            return node;                                                                                         \
        }                                                                                                        \
                                                                                                                 \
        while(1)                                                                                                 \
        {                                                                                                        \
            if(node->parent == NULL)                                                                             \
            {                                                                                                    \
                node = NULL;                                                                                     \
                return node;                                                                                     \
            }                                                                                                    \
            if(node->parent->left == node)                                                                       \
            {                                                                                                    \
                node = node->parent;                                                                             \
                return node;                                                                                     \
            }                                                                                                    \
            node = node->parent;                                                                                 \
        }                                                                                                        \
        return node;                                                                                             \
    }                                                                                                            \
                                                                                                                 \
    static container_t##_node * __##container_t##_GetPrevNode(container_t##_node * node)                         \
    {                                                                                                            \
        if(!node)                                                                                                \
        {                                                                                                        \
            return NULL;                                                                                         \
        }                                                                                                        \
        if(node->left != NULL)                                                                                   \
        {                                                                                                        \
            node = node->left;                                                                                   \
            while(node->right != NULL)                                                                           \
            {                                                                                                    \
                node = node->right;                                                                              \
            }                                                                                                    \
            return node;                                                                                         \
        }                                                                                                        \
                                                                                                                 \
        while(1)                                                                                                 \
        {                                                                                                        \
            if(node->parent == NULL)                                                                             \
            {                                                                                                    \
                node = NULL;                                                                                     \
                return node;                                                                                     \
            }                                                                                                    \
            if(node->parent->right == node)                                                                      \
            {                                                                                                    \
                node = node->parent;                                                                             \
                return node;                                                                                     \
            }                                                                                                    \
            node = node->parent;                                                                                 \
        }                                                                                                        \
        return node;                                                                                             \
    }

#define __static_binary_tree_methods_c(container_t, key_t, container_capacity)                                  \
    __private_set_methods_c(container_t, key_t);                                                                \
                                                                                                                \
    void container_t##_Construct(container_t * const self)                                                      \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->size = 0;                                                                                         \
        self->root = NULL;                                                                                      \
        container_t##_pool_Construct(&self->pool);                                                              \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Destruct(container_t * const self)                                                       \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_pool_Destruct(&self->pool);                                                               \
    }                                                                                                           \
                                                                                                                \
    size_t container_t##_Size(const container_t * const self)                                                   \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Empty(const container_t * const self)                                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size == 0;                                                                                 \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Insert(container_t * const self, key_t data)                                              \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * node = container_t##_pool_Allocate(&self->pool);                                   \
        if(node)                                                                                                \
        {                                                                                                       \
            node->key = data;                                                                                   \
                                                                                                                \
            if(self->root)                                                                                      \
            {                                                                                                   \
                container_t##_node * parent_node = NULL;                                                        \
                container_t##_node * child_node  = self->root;                                                  \
                                                                                                                \
                while(1)                                                                                        \
                {                                                                                               \
                    parent_node = child_node;                                                                   \
                                                                                                                \
                    int compare_value = key_t##_Compare((const key_t *)&data, (const key_t *)&child_node->key); \
                    if(compare_value < 0)                                                                       \
                    {                                                                                           \
                        child_node = child_node->left;                                                          \
                        if(!child_node)                                                                         \
                        {                                                                                       \
                            parent_node->left = node;                                                           \
                            break;                                                                              \
                        }                                                                                       \
                    }                                                                                           \
                    else if(compare_value > 0)                                                                  \
                    {                                                                                           \
                        child_node = child_node->right;                                                         \
                        if(!child_node)                                                                         \
                        {                                                                                       \
                            parent_node->right = node;                                                          \
                            break;                                                                              \
                        }                                                                                       \
                    }                                                                                           \
                    else                                                                                        \
                    {                                                                                           \
                        container_t##_pool_Release(&self->pool, node);                                          \
                        return ELEMENT_EXISTS;                                                                  \
                    }                                                                                           \
                }                                                                                               \
                node->right  = NULL;                                                                            \
                node->left   = NULL;                                                                            \
                node->parent = parent_node;                                                                     \
                                                                                                                \
                node->color = RED;                                                                              \
                if(node->parent && node->parent->parent && node->parent->color == RED)                          \
                {                                                                                               \
                    __##container_t##_BlackRedTreeBalance(self, node);                                          \
                }                                                                                               \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                node->right  = NULL;                                                                            \
                node->left   = NULL;                                                                            \
                node->parent = NULL;                                                                            \
                self->root   = node;                                                                            \
            }                                                                                                   \
            self->root->color = BLACK;                                                                          \
            ++self->size;                                                                                       \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                  \
    {                                                                                                           \
        printf("Erase node %p (%d) (%d)\n", iterator->node, self->size, iterator->node->key);                   \
        container_t##_Iterator it  = container_t##_Begin(self);                                                 \
        container_t##_Iterator end = container_t##_End(self);                                                   \
        for(; !container_t##_Iterator_Equal(&it, &end); container_t##_Iterator_Increment(&it))                  \
        {                                                                                                       \
            container_t##_node * node = it.node;                                                                \
            while(node)                                                                                         \
            {                                                                                                   \
                printf("%d (%s)", node->key, node->color == RED ? "RED" : "BLACK");                             \
                if(node->parent && node->parent->left == node)                                                  \
                {                                                                                               \
                    printf(" -L-> ");                                                                           \
                }                                                                                               \
                if(node->parent && node->parent->right == node)                                                 \
                {                                                                                               \
                    printf(" -R-> ");                                                                           \
                }                                                                                               \
                node = node->parent;                                                                            \
            }                                                                                                   \
            printf("\n");                                                                                       \
        }                                                                                                       \
        assert(self);                                                                                           \
        container_t##_node * node = iterator->node;                                                             \
        container_t##_node *target, *child;                                                                     \
        key_t * data;                                                                                           \
                                                                                                                \
        data = &node->key;                                                                                      \
                                                                                                                \
        /* choose node's in-order successor if it has two children */                                           \
                                                                                                                \
        if(node->left == NULL || node->right == NULL)                                                           \
        {                                                                                                       \
            printf("Has at least 1 NULL\n");                                                                    \
            target = node;                                                                                      \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            printf("Has 2 children\n");                                                                         \
            target = __##container_t##_Successor(self, node); /* node->right must not be NIL, thus move down */ \
            printf("successor: %p\n", target);                                                                  \
            node->key = target->key; /* data swapped */                                                         \
        }                                                                                                       \
                                                                                                                \
        child = (target->left == NULL) ? target->right : target->left; /* child may be NIL */                   \
                                                                                                                \
        if(target->color == BLACK)                                                                              \
        {                                                                                                       \
            printf("Target is black\n");                                                                        \
            if(child && child->color == RED)                                                                    \
            {                                                                                                   \
                /* deletion from 3-children cluster (BLACK target node, RED child node) */                      \
                child->color = BLACK;                                                                           \
            }                                                                                                   \
            else if(target == self->root)                                                                       \
            {                                                                                                   \
                /* deletion from 2-children root cluster (BLACK target node, BLACK child node) */               \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                /* deletion from 2-children cluster (BLACK target node, ...) */                                 \
                __##container_t##_BalanceErase(self, target);                                                   \
            }                                                                                                   \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            printf("Target is red\n");                                                                          \
            /* deletion from 4-children cluster (RED target node) */                                            \
            /* deletion from 3-children cluster (RED target node) */                                            \
        }                                                                                                       \
        printf("Check child\n");                                                                                \
        if(child != NULL)                                                                                       \
            child->parent = target->parent;                                                                     \
        if(target->parent)                                                                                      \
        {                                                                                                       \
            printf("Has parent\n");                                                                             \
            if(target == target->parent->left)                                                                  \
                target->parent->left = child;                                                                   \
            else                                                                                                \
                target->parent->right = child;                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            printf("Has no parent\n");                                                                          \
            self->root = child;                                                                                 \
        }                                                                                                       \
                                                                                                                \
        container_t##_pool_Release(&self->pool, target);                                                        \
                                                                                                                \
        --self->size;                                                                                           \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it = { 0 };                                                                      \
                                                                                                                \
        /* Go to the last left node */                                                                          \
        container_t##_node * parent_node = NULL;                                                                \
        container_t##_node * child_node  = self->root;                                                          \
        while(child_node)                                                                                       \
        {                                                                                                       \
            parent_node = child_node;                                                                           \
            child_node  = child_node->left;                                                                     \
        }                                                                                                       \
        it.node = parent_node;                                                                                  \
        if(parent_node)                                                                                         \
        {                                                                                                       \
            if(parent_node->right)                                                                              \
            {                                                                                                   \
                it.next = parent_node->right;                                                                   \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                it.next = parent_node->parent;                                                                  \
            }                                                                                                   \
        }                                                                                                       \
        it.prev = NULL;                                                                                         \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_End(const container_t * const self)                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it = { 0 };                                                                      \
                                                                                                                \
        /* Go to the last right node */                                                                         \
        container_t##_node * parent_node = NULL;                                                                \
        container_t##_node * child_node  = self->root;                                                          \
        while(child_node)                                                                                       \
        {                                                                                                       \
            parent_node = child_node;                                                                           \
            child_node  = child_node->right;                                                                    \
        }                                                                                                       \
        it.prev = parent_node;                                                                                  \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                               \
                                      const container_t##_Iterator * const second)                              \
    {                                                                                                           \
        return first->node == second->node;                                                                     \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->prev = self->node;                                                                                \
        self->node = self->next;                                                                                \
        if(!self->next)                                                                                         \
        {                                                                                                       \
            return;                                                                                             \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            self->next = __##container_t##_GetNextNode(self->node);                                             \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->next = self->node;                                                                                \
        self->node = self->prev;                                                                                \
        if(!self->prev)                                                                                         \
        {                                                                                                       \
            return;                                                                                             \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            self->prev = __##container_t##_GetPrevNode(self->node);                                             \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    const key_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                        \
    {                                                                                                           \
        return (const key_t *)&self->node->key;                                                                 \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t data)                             \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it = { 0 };                                                                      \
                                                                                                                \
        container_t##_node * node = self->root;                                                                 \
                                                                                                                \
        while(node)                                                                                             \
        {                                                                                                       \
            int compare_result = key_t##_Compare((const key_t *)&data, (const key_t *)&node->key);              \
            if(compare_result == 0)                                                                             \
            {                                                                                                   \
                break;                                                                                          \
            }                                                                                                   \
            else if(compare_result < 0)                                                                         \
            {                                                                                                   \
                node = node->left;                                                                              \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                node = node->right;                                                                             \
            }                                                                                                   \
        }                                                                                                       \
        it.node = node;                                                                                         \
        it.next = __##container_t##_GetNextNode(node);                                                          \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Clear(container_t * const self)                                                          \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        while(container_t##_Size(self) != 0)                                                                    \
        {                                                                                                       \
            container_t##_Iterator begin = container_t##_Begin(self);                                           \
            container_t##_Erase(self, &begin);                                                                  \
        }                                                                                                       \
    }

#define __custom_allocator_binary_tree_methods_c(container_t, key_t, allocator_t)                               \
    __private_set_methods_c(container_t, key_t);                                                                \
                                                                                                                \
    void container_t##_Construct(container_t * const self)                                                      \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->size = 0;                                                                                         \
        self->root = NULL;                                                                                      \
        allocator_t##_Construct(&self->allocator);                                                              \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Destruct(container_t * const self)                                                       \
    {                                                                                                           \
        while(container_t##_Size(self) != 0)                                                                    \
        {                                                                                                       \
            container_t##_Iterator begin = container_t##_Begin(self);                                           \
            container_t##_Erase(self, &begin);                                                                  \
        }                                                                                                       \
        allocator_t##_Destruct(&self->allocator);                                                               \
    }                                                                                                           \
                                                                                                                \
    size_t container_t##_Size(const container_t * const self)                                                   \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Empty(const container_t * const self)                                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        return self->size == 0;                                                                                 \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Insert(container_t * const self, key_t data)                                              \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * node =                                                                             \
            (container_t##_node *)allocator_t##_Allocate(&self->allocator, sizeof(container_t##_node));         \
        if(node)                                                                                                \
        {                                                                                                       \
            node->key = data;                                                                                   \
                                                                                                                \
            if(self->root)                                                                                      \
            {                                                                                                   \
                container_t##_node * parent_node = NULL;                                                        \
                container_t##_node * child_node  = self->root;                                                  \
                                                                                                                \
                while(1)                                                                                        \
                {                                                                                               \
                    parent_node = child_node;                                                                   \
                                                                                                                \
                    int compare_value = key_t##_Compare((const key_t *)&data, (const key_t *)&child_node->key); \
                    if(compare_value < 0)                                                                       \
                    {                                                                                           \
                        child_node = child_node->left;                                                          \
                        if(!child_node)                                                                         \
                        {                                                                                       \
                            parent_node->left = node;                                                           \
                            break;                                                                              \
                        }                                                                                       \
                    }                                                                                           \
                    else if(compare_value > 0)                                                                  \
                    {                                                                                           \
                        child_node = child_node->right;                                                         \
                        if(!child_node)                                                                         \
                        {                                                                                       \
                            parent_node->right = node;                                                          \
                            break;                                                                              \
                        }                                                                                       \
                    }                                                                                           \
                    else                                                                                        \
                    {                                                                                           \
                        allocator_t##_Deallocate(&self->allocator, node);                                       \
                        return ELEMENT_EXISTS;                                                                  \
                    }                                                                                           \
                }                                                                                               \
                node->right  = NULL;                                                                            \
                node->left   = NULL;                                                                            \
                node->parent = parent_node;                                                                     \
                node->color  = RED;                                                                             \
                if(node->parent && node->parent->parent && node->parent->color == RED)                          \
                {                                                                                               \
                    __##container_t##_BlackRedTreeBalance(self, node);                                          \
                }                                                                                               \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                node->right  = NULL;                                                                            \
                node->left   = NULL;                                                                            \
                node->parent = NULL;                                                                            \
                self->root   = node;                                                                            \
            }                                                                                                   \
            self->root->color = BLACK;                                                                          \
            ++self->size;                                                                                       \
                                                                                                                \
            return self->size;                                                                                  \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            return ALLOCATION_ERROR;                                                                            \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    int container_t##_Erase(container_t * const self, container_t##_Iterator * const iterator)                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_node * to_delete_node            = iterator->node;                                        \
        container_t##_node * to_delete_node_parent     = iterator->node->parent;                                \
        container_t##_node * new_node_at_this_Iterator = NULL;                                                  \
        assert(to_delete_node);                                                                                 \
                                                                                                                \
        if(!to_delete_node->left && !to_delete_node->right)                                                     \
        {                                                                                                       \
            /* The node has no children */                                                                      \
            new_node_at_this_Iterator = NULL;                                                                   \
        }                                                                                                       \
        else if(to_delete_node->left && to_delete_node->right)                                                  \
        {                                                                                                       \
            /* The node has 2 children */                                                                       \
                                                                                                                \
            /* Find minimum in right subtree */                                                                 \
            container_t##_node * minimum_node_in_right_subtree = to_delete_node->right;                         \
            while(minimum_node_in_right_subtree->left)                                                          \
            {                                                                                                   \
                minimum_node_in_right_subtree = minimum_node_in_right_subtree->left;                            \
            }                                                                                                   \
            /* Copy mininum key into to delete node */                                                          \
            assert(minimum_node_in_right_subtree);                                                              \
            to_delete_node->key = minimum_node_in_right_subtree->key;                                           \
                                                                                                                \
            /* Set to_delete_node pointer on new node */                                                        \
            to_delete_node        = minimum_node_in_right_subtree;                                              \
            to_delete_node_parent = to_delete_node->parent;                                                     \
                                                                                                                \
            /* Verify if to delete node has right node */                                                       \
            if(to_delete_node->right)                                                                           \
            {                                                                                                   \
                new_node_at_this_Iterator = to_delete_node->right;                                              \
            }                                                                                                   \
        }                                                                                                       \
        else if(to_delete_node->left)                                                                           \
        {                                                                                                       \
            /* The node has only left child */                                                                  \
            new_node_at_this_Iterator    = to_delete_node->left;                                                \
            to_delete_node->left->parent = to_delete_node_parent;                                               \
        }                                                                                                       \
        else if(to_delete_node->right)                                                                          \
        {                                                                                                       \
            /* The node has only right child */                                                                 \
            new_node_at_this_Iterator     = to_delete_node->right;                                              \
            to_delete_node->right->parent = to_delete_node_parent;                                              \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            assert(0);                                                                                          \
        }                                                                                                       \
                                                                                                                \
        if(to_delete_node == self->root)                                                                        \
        {                                                                                                       \
            self->root = new_node_at_this_Iterator;                                                             \
            if(self->root)                                                                                      \
            {                                                                                                   \
                self->root->parent = NULL;                                                                      \
            }                                                                                                   \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            const bool is_to_delete_node_left = to_delete_node_parent->left == to_delete_node;                  \
                                                                                                                \
            if(is_to_delete_node_left)                                                                          \
            {                                                                                                   \
                to_delete_node_parent->left = new_node_at_this_Iterator;                                        \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                to_delete_node_parent->right = new_node_at_this_Iterator;                                       \
            }                                                                                                   \
        }                                                                                                       \
                                                                                                                \
        allocator_t##_Deallocate(&self->allocator, to_delete_node);                                             \
                                                                                                                \
        --self->size;                                                                                           \
        return self->size;                                                                                      \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Begin(const container_t * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it = { 0 };                                                                      \
                                                                                                                \
        /* Go to the last left node */                                                                          \
        container_t##_node * parent_node = NULL;                                                                \
        container_t##_node * child_node  = self->root;                                                          \
        while(child_node)                                                                                       \
        {                                                                                                       \
            parent_node = child_node;                                                                           \
            child_node  = child_node->left;                                                                     \
        }                                                                                                       \
        it.node = parent_node;                                                                                  \
        if(parent_node)                                                                                         \
        {                                                                                                       \
            if(parent_node->right)                                                                              \
            {                                                                                                   \
                it.next = parent_node->right;                                                                   \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                it.next = parent_node->parent;                                                                  \
            }                                                                                                   \
        }                                                                                                       \
        it.prev = NULL;                                                                                         \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_End(const container_t * const self)                                    \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it = { 0 };                                                                      \
                                                                                                                \
        /* Go to the last right node */                                                                         \
        container_t##_node * parent_node = NULL;                                                                \
        container_t##_node * child_node  = self->root;                                                          \
        while(child_node)                                                                                       \
        {                                                                                                       \
            parent_node = child_node;                                                                           \
            child_node  = child_node->right;                                                                    \
        }                                                                                                       \
        it.prev = parent_node;                                                                                  \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    bool container_t##_Iterator_Equal(const container_t##_Iterator * const first,                               \
                                      const container_t##_Iterator * const second)                              \
    {                                                                                                           \
        return first->node == second->node;                                                                     \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Increment(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->prev = self->node;                                                                                \
        self->node = self->next;                                                                                \
        if(!self->next)                                                                                         \
        {                                                                                                       \
            return;                                                                                             \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            self->next = __##container_t##_GetNextNode(self->node);                                             \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Iterator_Decrement(container_t##_Iterator * const self)                                  \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        self->next = self->node;                                                                                \
        self->node = self->prev;                                                                                \
        if(!self->prev)                                                                                         \
        {                                                                                                       \
            return;                                                                                             \
        }                                                                                                       \
        else                                                                                                    \
        {                                                                                                       \
            self->prev = __##container_t##_GetPrevNode(self->node);                                             \
        }                                                                                                       \
    }                                                                                                           \
                                                                                                                \
    const key_t * container_t##_Iterator_CRef(const container_t##_Iterator * const self)                        \
    {                                                                                                           \
        return (const key_t *)&self->node->key;                                                                 \
    }                                                                                                           \
                                                                                                                \
    container_t##_Iterator container_t##_Find(container_t * const self, key_t data)                             \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        container_t##_Iterator it = { 0 };                                                                      \
                                                                                                                \
        container_t##_node * node = self->root;                                                                 \
                                                                                                                \
        while(node)                                                                                             \
        {                                                                                                       \
            int compare_result = key_t##_Compare((const key_t *)&data, (const key_t *)&node->key);              \
            if(compare_result == 0)                                                                             \
            {                                                                                                   \
                break;                                                                                          \
            }                                                                                                   \
            else if(compare_result < 0)                                                                         \
            {                                                                                                   \
                node = node->left;                                                                              \
            }                                                                                                   \
            else                                                                                                \
            {                                                                                                   \
                node = node->right;                                                                             \
            }                                                                                                   \
        }                                                                                                       \
        it.node = node;                                                                                         \
        it.next = __##container_t##_GetNextNode(node);                                                          \
        return it;                                                                                              \
    }                                                                                                           \
                                                                                                                \
    void container_t##_Clear(container_t * const self)                                                          \
    {                                                                                                           \
        assert(self);                                                                                           \
                                                                                                                \
        while(container_t##_Size(self) != 0)                                                                    \
        {                                                                                                       \
            container_t##_Iterator begin = container_t##_Begin(self);                                           \
            container_t##_Erase(self, &begin);                                                                  \
        }                                                                                                       \
    }

#define static_set(container_t, key_t, container_capacity)                   \
    typedef struct container_t container_t;                                  \
    typedef struct container_t##_Iterator container_t##_Iterator;            \
    typedef struct container_t##_node container_t##_node;                    \
    typedef int (*container_t##_compare_t)(const key_t *, const key_t *);    \
                                                                             \
    struct container_t##_node                                                \
    {                                                                        \
        container_t##_node * right;                                          \
        container_t##_node * left;                                           \
        container_t##_node * parent;                                         \
        int color;                                                           \
        key_t key;                                                           \
    };                                                                       \
                                                                             \
    struct container_t##_Iterator                                            \
    {                                                                        \
        container_t##_node * node;                                           \
        container_t##_node * next;                                           \
        container_t##_node * prev;                                           \
    };                                                                       \
    static_pool(container_t##_pool, container_t##_node, container_capacity); \
                                                                             \
    struct container_t                                                       \
    {                                                                        \
        container_t##_node * root;                                           \
        container_t##_pool pool;                                             \
        container_t##_compare_t compare_function;                            \
        size_t size;                                                         \
    };                                                                       \
    __binary_tree_methods_h(container_t, key_t)

#define static_set_impl(container_t, key_t, container_capacity)                   \
    static_pool_impl(container_t##_pool, container_t##_node, container_capacity); \
    __static_binary_tree_methods_c(container_t, key_t, container_capacity)

#define custom_allocator_set(container_t, key_t, allocator_t)             \
    typedef struct container_t container_t;                               \
    typedef struct container_t##_Iterator container_t##_Iterator;         \
    typedef struct container_t##_node container_t##_node;                 \
    typedef int (*container_t##_compare_t)(const key_t *, const key_t *); \
                                                                          \
    struct container_t##_node                                             \
    {                                                                     \
        container_t##_node * right;                                       \
        container_t##_node * left;                                        \
        container_t##_node * parent;                                      \
        int color;                                                        \
        key_t key;                                                        \
    };                                                                    \
                                                                          \
    struct container_t##_Iterator                                         \
    {                                                                     \
        container_t##_node * node;                                        \
        container_t##_node * next;                                        \
        container_t##_node * prev;                                        \
    };                                                                    \
                                                                          \
    struct container_t                                                    \
    {                                                                     \
        container_t##_node * root;                                        \
        allocator_t allocator;                                            \
        container_t##_compare_t compare_function;                         \
        size_t size;                                                      \
    };                                                                    \
    __binary_tree_methods_h(container_t, key_t);

#define custom_allocator_set_impl(container_t, key_t, allocator_t) \
    __custom_allocator_binary_tree_methods_c(container_t, key_t, allocator_t)

#define dynamic_set(container_t, key_t)             \
    dynamic_memory(container_t##_DynamicAllocator); \
    custom_allocator_set(container_t, key_t, container_t##_DynamicAllocator);

#define dynamic_set_impl(container_t, key_t)             \
    dynamic_memory_impl(container_t##_DynamicAllocator); \
    custom_allocator_set_impl(container_t, key_t, container_t##_DynamicAllocator);

#ifdef __cplusplus
}
#endif