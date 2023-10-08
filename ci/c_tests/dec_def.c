#include "dec.h"

int main()
{
    StaticVector staticVector;
    StaticVector_Construct(&staticVector);
    StaticVector_Iterator staticVectorIt = StaticVector_Begin(&staticVector);
    StaticVector_Destroy(&staticVector);

    DynamicVector dynamicVector;
    DynamicVector_Construct(&dynamicVector);
    DynamicVector_Iterator dynamicVectorIt = DynamicVector_Begin(&dynamicVector);
    DynamicVector_Destroy(&dynamicVector);

    CustomAllocatorVector customAllocatorVector;
    CustomAllocatorVector_Construct(&customAllocatorVector);
    CustomAllocatorVector_Iterator customAllocatorVectorIt = CustomAllocatorVector_Begin(&customAllocatorVector);
    CustomAllocatorVector_Destroy(&customAllocatorVector);

    StaticList staticList;
    StaticList_Construct(&staticList);
    StaticList_Iterator staticListIt = StaticList_Begin(&staticList);
    StaticList_Destroy(&staticList);

    DynamicList dynamicList;
    DynamicList_Construct(&dynamicList);
    DynamicList_Iterator dynamicListIt = DynamicList_Begin(&dynamicList);
    DynamicList_Destroy(&dynamicList);

    CustomAllocatorList customAllocatorList;
    CustomAllocatorList_Construct(&customAllocatorList);
    CustomAllocatorList_Iterator customAllocatorListIt = CustomAllocatorList_Begin(&customAllocatorList);
    CustomAllocatorList_Destroy(&customAllocatorList);

    StaticDeque staticDeque;
    StaticDeque_Construct(&staticDeque);
    StaticDeque_Iterator staticDequeIt = StaticDeque_Begin(&staticDeque);
    StaticDeque_Destroy(&staticDeque);

    StaticSet staticSet;
    StaticSet_Construct(&staticSet);
    StaticSet_Iterator staticSetIt = StaticSet_Begin(&staticSet);
    StaticSet_Destroy(&staticSet);

    CustomAllocatorSet customAllocatorSet;
    CustomAllocatorSet_Construct(&customAllocatorSet);
    CustomAllocatorSet_Iterator customAllocatorSetIt = CustomAllocatorSet_Begin(&customAllocatorSet);
    CustomAllocatorSet_Destroy(&customAllocatorSet);

    StaticUnorderedSet staticUnorderedSet;
    StaticUnorderedSet_Construct(&staticUnorderedSet);
    StaticUnorderedSet_Iterator staticUnorderedSetIt = StaticUnorderedSet_Begin(&staticUnorderedSet);
    StaticUnorderedSet_Destroy(&staticUnorderedSet);

    CustomAllocatorUnorderedSet customAllocatorUnorderedSet;
    CustomAllocatorUnorderedSet_Construct(&customAllocatorUnorderedSet);
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