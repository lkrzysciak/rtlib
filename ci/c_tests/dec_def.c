#include "dec.h"

int main()
{
    StaticVector staticVector;
    StaticVector_Construct(&staticVector, NULL);
    StaticVector_Iterator staticVectorIt = StaticVector_Begin(&staticVector);
    StaticVector_Destroy(&staticVector);

    DynamicVector dynamicVector;
    DynamicVector_Construct(&dynamicVector, NULL);
    DynamicVector_Iterator dynamicVectorIt = DynamicVector_Begin(&dynamicVector);
    DynamicVector_Destroy(&dynamicVector);

    CustomAllocatorVector customAllocatorVector;
    CustomAllocatorVector_Construct(&customAllocatorVector, NULL);
    CustomAllocatorVector_Iterator customAllocatorVectorIt = CustomAllocatorVector_Begin(&customAllocatorVector);
    CustomAllocatorVector_Destroy(&customAllocatorVector);

    return 0;
}