#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>


typedef struct Vector Vector;
typedef struct VectorIterator VectorIterator;

Vector* Vector_Init(void * buffer, size_t size, size_t element_size);
size_t Vector_Size(Vector * const self);

int Vector_PushBack(Vector * const self, const void * const data);
int Vector_PopBack(Vector * const self);
int Vector_PushFront(Vector * const self, const void * const data);
int Vector_PopFront(Vector * const self);

const void * Vector_Front(Vector * const self);
const void * Vector_Back(Vector * const self);


#ifdef __cplusplus
}
#endif