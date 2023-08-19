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

    StaticList staticList;
    StaticList_Construct(&staticList, NULL);
    StaticList_Iterator staticListIt = StaticList_Begin(&staticList);
    StaticList_Destroy(&staticList);

    DynamicList dynamicList;
    DynamicList_Construct(&dynamicList, NULL);
    DynamicList_Iterator dynamicListIt = DynamicList_Begin(&dynamicList);
    DynamicList_Destroy(&dynamicList);

    CustomAllocatorList customAllocatorList;
    CustomAllocatorList_Construct(&customAllocatorList, NULL);
    CustomAllocatorList_Iterator customAllocatorListIt = CustomAllocatorList_Begin(&customAllocatorList);
    CustomAllocatorList_Destroy(&customAllocatorList);

    StaticDeque staticDeque;
    StaticDeque_Construct(&staticDeque, NULL);
    StaticDeque_Iterator staticDequeIt = StaticDeque_Begin(&staticDeque);
    StaticDeque_Destroy(&staticDeque);

    return 0;
}