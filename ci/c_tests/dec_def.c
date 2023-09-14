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

    StaticSet staticSet;
    StaticSet_Construct(&staticSet, NULL);
    StaticSet_Iterator staticSetIt = StaticSet_Begin(&staticSet);
    StaticSet_Destroy(&staticSet);

    CustomAllocatorSet customAllocatorSet;
    CustomAllocatorSet_Construct(&customAllocatorSet, NULL);
    CustomAllocatorSet_Iterator customAllocatorSetIt = CustomAllocatorSet_Begin(&customAllocatorSet);
    CustomAllocatorSet_Destroy(&customAllocatorSet);

    StaticUnorderedSet staticUnorderedSet;
    StaticUnorderedSet_Construct(&staticUnorderedSet, NULL, NULL);
    StaticUnorderedSet_Iterator staticUnorderedSetIt = StaticUnorderedSet_Begin(&staticUnorderedSet);
    StaticUnorderedSet_Destroy(&staticUnorderedSet);

    CustomAllocatorUnorderedSet customAllocatorUnorderedSet;
    CustomAllocatorUnorderedSet_Construct(&customAllocatorUnorderedSet, NULL, NULL);
    CustomAllocatorUnorderedSet_Iterator customAllocatorUnorderedSetIt =
        CustomAllocatorUnorderedSet_Begin(&customAllocatorUnorderedSet);
    CustomAllocatorUnorderedSet_Destroy(&customAllocatorUnorderedSet);

    StaticMap staticMap;
    StaticMap_Construct(&staticMap);
    StaticMap_Iterator staticMapIt = StaticMap_Begin(&staticMap);
    StaticMap_Destruct(&staticMap);

    CustomAllocatorMap customAllocatorMap;
    CustomAllocatorMap_Construct(&customAllocatorMap);
    CustomAllocatorMap_Iterator customAllocatorMapIt = CustomAllocatorMap_Begin(&customAllocatorMap);
    CustomAllocatorMap_Destruct(&customAllocatorMap);

    DynamicMap dynamicMap;
    DynamicMap_Construct(&dynamicMap);
    DynamicMap_Iterator dynamicMapIt = DynamicMap_Begin(&dynamicMap);
    DynamicMap_Destruct(&dynamicMap);

    return 0;
}