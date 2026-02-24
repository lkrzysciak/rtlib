#include "dec.h"

int main()
{
    StaticVector staticVector;
    StaticVector_Construct(&staticVector);
    StaticVector_Iterator staticVectorIt = StaticVector_Begin(&staticVector);
    (void)staticVectorIt;
    StaticVector_Destruct(&staticVector);

    DynamicVector dynamicVector;
    DynamicVector_Construct(&dynamicVector);
    DynamicVector_Iterator dynamicVectorIt = DynamicVector_Begin(&dynamicVector);
    (void)dynamicVectorIt;
    DynamicVector_Destruct(&dynamicVector);

    CustomAllocatorVector customAllocatorVector;
    CustomAllocatorVector_Construct(&customAllocatorVector);
    CustomAllocatorVector_Iterator customAllocatorVectorIt = CustomAllocatorVector_Begin(&customAllocatorVector);
    (void)customAllocatorVectorIt;
    CustomAllocatorVector_Destruct(&customAllocatorVector);

    StaticList staticList;
    StaticList_Construct(&staticList);
    StaticList_Iterator staticListIt = StaticList_Begin(&staticList);
    (void)staticListIt;
    StaticList_Destruct(&staticList);

    DynamicList dynamicList;
    DynamicList_Construct(&dynamicList);
    DynamicList_Iterator dynamicListIt = DynamicList_Begin(&dynamicList);
    (void)dynamicListIt;
    DynamicList_Destruct(&dynamicList);

    CustomAllocatorList customAllocatorList;
    CustomAllocatorList_Construct(&customAllocatorList);
    CustomAllocatorList_Iterator customAllocatorListIt = CustomAllocatorList_Begin(&customAllocatorList);
    (void)customAllocatorListIt;
    CustomAllocatorList_Destruct(&customAllocatorList);

    StaticDeque staticDeque;
    StaticDeque_Construct(&staticDeque);
    StaticDeque_Iterator staticDequeIt = StaticDeque_Begin(&staticDeque);
    (void)staticDequeIt;
    StaticDeque_Destruct(&staticDeque);

    StaticSet staticSet;
    StaticSet_Construct(&staticSet);
    StaticSet_Iterator staticSetIt = StaticSet_Begin(&staticSet);
    (void)staticSetIt;
    StaticSet_Destruct(&staticSet);

    CustomAllocatorSet customAllocatorSet;
    CustomAllocatorSet_Construct(&customAllocatorSet);
    CustomAllocatorSet_Iterator customAllocatorSetIt = CustomAllocatorSet_Begin(&customAllocatorSet);
    (void)customAllocatorSetIt;
    CustomAllocatorSet_Destruct(&customAllocatorSet);

    StaticUnorderedSet staticUnorderedSet;
    StaticUnorderedSet_Construct(&staticUnorderedSet);
    StaticUnorderedSet_Iterator staticUnorderedSetIt = StaticUnorderedSet_Begin(&staticUnorderedSet);
    (void)staticUnorderedSetIt;
    StaticUnorderedSet_Destruct(&staticUnorderedSet);

    CustomAllocatorUnorderedSet customAllocatorUnorderedSet;
    CustomAllocatorUnorderedSet_Construct(&customAllocatorUnorderedSet);
    CustomAllocatorUnorderedSet_Iterator customAllocatorUnorderedSetIt =
        CustomAllocatorUnorderedSet_Begin(&customAllocatorUnorderedSet);
    (void)customAllocatorUnorderedSetIt;
    CustomAllocatorUnorderedSet_Destruct(&customAllocatorUnorderedSet);

    StaticMap staticMap;
    StaticMap_Construct(&staticMap);
    StaticMap_Iterator staticMapIt = StaticMap_Begin(&staticMap);
    (void)staticMapIt;
    StaticMap_Destruct(&staticMap);

    CustomAllocatorMap customAllocatorMap;
    CustomAllocatorMap_Construct(&customAllocatorMap);
    CustomAllocatorMap_Iterator customAllocatorMapIt = CustomAllocatorMap_Begin(&customAllocatorMap);
    (void)customAllocatorMapIt;
    CustomAllocatorMap_Destruct(&customAllocatorMap);

    DynamicMap dynamicMap;
    DynamicMap_Construct(&dynamicMap);
    DynamicMap_Iterator dynamicMapIt = DynamicMap_Begin(&dynamicMap);
    (void)dynamicMapIt;
    DynamicMap_Destruct(&dynamicMap);

    StaticUnorderedMap staticUnorderedMap;
    StaticUnorderedMap_Construct(&staticUnorderedMap);
    StaticUnorderedMap_Iterator staticUnorderedMapIt = StaticUnorderedMap_Begin(&staticUnorderedMap);
    (void)staticUnorderedMapIt;
    StaticUnorderedMap_Destruct(&staticUnorderedMap);

    CustomAllocatorUnorderedMap customAllocatorUnorderedMap;
    CustomAllocatorUnorderedMap_Construct(&customAllocatorUnorderedMap);
    CustomAllocatorUnorderedMap_Iterator customAllocatorUnorderedMapIt =
        CustomAllocatorUnorderedMap_Begin(&customAllocatorUnorderedMap);
    (void)customAllocatorUnorderedMapIt;
    CustomAllocatorUnorderedMap_Destruct(&customAllocatorUnorderedMap);

    DynamicUnorderedMap dynamicUnorderedMap;
    DynamicUnorderedMap_Construct(&dynamicUnorderedMap);
    DynamicUnorderedMap_Iterator dynamicUnorderedMapIt = DynamicUnorderedMap_Begin(&dynamicUnorderedMap);
    (void)dynamicUnorderedMapIt;
    DynamicUnorderedMap_Destruct(&dynamicUnorderedMap);

    return 0;
}