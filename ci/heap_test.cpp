#include "gtest/gtest.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

extern "C"
{
#include "rtlib/heap.h"

    static_heap(StaticHeap, 256);
    static_heap_impl(StaticHeap, 256);

    dynamic_heap(DynamicHeap, 256);
    dynamic_heap_impl(DynamicHeap, 256);
}

#define create_wrappers_for_type(Type)                                  \
    void Init(Type * const container)                                   \
    {                                                                   \
        Type##_Construct(container);                                    \
    }                                                                   \
                                                                        \
    void Deinit(Type * const container)                                 \
    {                                                                   \
        Type##_Destruct(container);                                     \
    }                                                                   \
                                                                        \
    void * Allocate(Type * const container, size_t size)                \
    {                                                                   \
        return Type##_Allocate(container, size);                        \
    }                                                                   \
                                                                        \
    void * Callocate(Type * const container, size_t count, size_t size) \
    {                                                                   \
        return Type##_Callocate(container, count, size);                \
    }                                                                   \
                                                                        \
    void * Reallocate(Type * const container, void * ptr, size_t size)  \
    {                                                                   \
        return Type##_Reallocate(container, ptr, size);                 \
    }                                                                   \
                                                                        \
    void Deallocate(Type * const container, void * ptr)                 \
    {                                                                   \
        Type##_Deallocate(container, ptr);                              \
    }

create_wrappers_for_type(StaticHeap);
create_wrappers_for_type(DynamicHeap);

static size_t GetMaxFreeBlock(const StaticHeap * heap)
{
    size_t max_free = 0;
    for(StaticHeap_block * block = heap->head; block; block = block->next)
    {
        if(block->free && block->size > max_free)
        {
            max_free = block->size;
        }
    }
    return max_free;
}

static bool HasAdjacentFreeBlocks(const StaticHeap * heap)
{
    for(StaticHeap_block * block = heap->head; block && block->next; block = block->next)
    {
        if(block->free && block->next->free)
        {
            return true;
        }
    }
    return false;
}

template<typename T>
struct HeapTest : public testing::Test
{
    void SetUp() override { Init(&heap); }

    void TearDown() override { Deinit(&heap); }

    T heap;
};

using HeapTypes = testing::Types<StaticHeap, DynamicHeap>;
TYPED_TEST_SUITE(HeapTest, HeapTypes);

TYPED_TEST(HeapTest, AllocateAndFree)
{
    unsigned char * a = static_cast<unsigned char *>(Allocate(&this->heap, 32));
    unsigned char * b = static_cast<unsigned char *>(Allocate(&this->heap, 64));

    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);

    memset(a, 0xA5, 32);
    memset(b, 0x5A, 64);

    Deallocate(&this->heap, b);
    Deallocate(&this->heap, a);

    void * c = Allocate(&this->heap, 48);
    ASSERT_NE(c, nullptr);
    Deallocate(&this->heap, c);
}

TYPED_TEST(HeapTest, ReallocateGrowAndShrink)
{
    unsigned char * data = static_cast<unsigned char *>(Allocate(&this->heap, 32));
    ASSERT_NE(data, nullptr);

    for(size_t i = 0; i < 32; ++i)
    {
        data[i] = static_cast<unsigned char>(i);
    }

    data = static_cast<unsigned char *>(Reallocate(&this->heap, data, 96));
    ASSERT_NE(data, nullptr);
    for(size_t i = 0; i < 32; ++i)
    {
        ASSERT_EQ(data[i], static_cast<unsigned char>(i));
    }

    data = static_cast<unsigned char *>(Reallocate(&this->heap, data, 16));
    ASSERT_NE(data, nullptr);
    for(size_t i = 0; i < 16; ++i)
    {
        ASSERT_EQ(data[i], static_cast<unsigned char>(i));
    }

    Deallocate(&this->heap, data);
}

TYPED_TEST(HeapTest, AllocateOverCapacityFails)
{
    void * ptr = Allocate(&this->heap, 1024);
    ASSERT_EQ(ptr, nullptr);
}

TYPED_TEST(HeapTest, CallocateZeroesMemory)
{
    unsigned char * data = static_cast<unsigned char *>(Callocate(&this->heap, 8, 4));
    ASSERT_NE(data, nullptr);

    for(size_t i = 0; i < 32; ++i)
    {
        ASSERT_EQ(data[i], 0u);
    }

    Deallocate(&this->heap, data);
}

TYPED_TEST(HeapTest, CallocateOverflowReturnsNull)
{
    void * ptr = Callocate(&this->heap, SIZE_MAX, 2);
    ASSERT_EQ(ptr, nullptr);
}

TYPED_TEST(HeapTest, CallocateZeroCountReturnsNull)
{
    void * ptr = Callocate(&this->heap, 0, 16);
    ASSERT_EQ(ptr, nullptr);
}

TYPED_TEST(HeapTest, CallocateZeroSizeReturnsNull)
{
    void * ptr = Callocate(&this->heap, 8, 0);
    ASSERT_EQ(ptr, nullptr);
}

TYPED_TEST(HeapTest, AllocateZeroReturnsNull)
{
    void * ptr = Allocate(&this->heap, 0);
    ASSERT_EQ(ptr, nullptr);
}

TYPED_TEST(HeapTest, ReallocateNullAllocates)
{
    unsigned char * data = static_cast<unsigned char *>(Reallocate(&this->heap, nullptr, 32));
    ASSERT_NE(data, nullptr);
    data[0] = 0xAB;
    Deallocate(&this->heap, data);
}

TYPED_TEST(HeapTest, ReallocateToZeroFrees)
{
    unsigned char * data = static_cast<unsigned char *>(Allocate(&this->heap, 24));
    ASSERT_NE(data, nullptr);
    data = static_cast<unsigned char *>(Reallocate(&this->heap, data, 0));
    ASSERT_EQ(data, nullptr);
}

TYPED_TEST(HeapTest, DeallocateNullNoop)
{
    Deallocate(&this->heap, nullptr);
}

TYPED_TEST(HeapTest, ReallocateTooLargeFailsAndKeepsOld)
{
    unsigned char * data = static_cast<unsigned char *>(Allocate(&this->heap, 32));
    ASSERT_NE(data, nullptr);
    data[0] = 0x7E;

    void * resized = Reallocate(&this->heap, data, 4096);
    ASSERT_EQ(resized, nullptr);
    ASSERT_EQ(data[0], 0x7E);

    Deallocate(&this->heap, data);
}

TEST(StaticHeapTest, ReuseFreedBlock)
{
    StaticHeap heap{};
    Init(&heap);

    void * a = Allocate(&heap, 24);
    void * b = Allocate(&heap, 24);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);

    Deallocate(&heap, b);
    void * c = Allocate(&heap, 24);
    ASSERT_EQ(c, b);

    Deallocate(&heap, a);
    Deallocate(&heap, c);
    Deinit(&heap);
}

TEST(StaticHeapTest, CoalesceAdjacentBlocksAllowsLargerAlloc)
{
    StaticHeap heap{};
    Init(&heap);

    void * a = Allocate(&heap, 32);
    void * b = Allocate(&heap, 32);
    void * c = Allocate(&heap, 32);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    ASSERT_NE(c, nullptr);

    Deallocate(&heap, b);
    Deallocate(&heap, a);

    void * merged = Allocate(&heap, 64);
    ASSERT_NE(merged, nullptr);

    Deallocate(&heap, c);
    Deallocate(&heap, merged);
    Deinit(&heap);
}

TEST(StaticHeapTest, CoalesceAcrossThreeBlocks)
{
    StaticHeap heap{};
    Init(&heap);

    void * a = Allocate(&heap, 32);
    void * b = Allocate(&heap, 32);
    void * c = Allocate(&heap, 32);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    ASSERT_NE(c, nullptr);

    Deallocate(&heap, b);
    Deallocate(&heap, c);
    Deallocate(&heap, a);

    void * merged = Allocate(&heap, 96);
    ASSERT_NE(merged, nullptr);

    Deallocate(&heap, merged);
    Deinit(&heap);
}

TEST(StaticHeapTest, FragmentationPreventsLargeAllocUntilFreed)
{
    StaticHeap heap{};
    Init(&heap);

    std::vector<void *> blocks;
    for(int i = 0; i < 3; ++i)
    {
        void * ptr = Allocate(&heap, 16);
        ASSERT_NE(ptr, nullptr);
        blocks.push_back(ptr);
    }

    while(true)
    {
        void * ptr = Allocate(&heap, 16);
        if(!ptr)
        {
            break;
        }
        blocks.push_back(ptr);
    }

    Deallocate(&heap, blocks[0]);
    Deallocate(&heap, blocks[2]);

    size_t total_free  = 0;
    size_t max_free    = 0;
    size_t free_blocks = 0;
    for(StaticHeap_block * block = heap.head; block; block = block->next)
    {
        if(block->free)
        {
            ++free_blocks;
            total_free += block->size;
            if(block->size > max_free)
            {
                max_free = block->size;
            }
        }
    }

    ASSERT_GE(free_blocks, 2u);
    size_t align   = sizeof(std::max_align_t);
    size_t request = max_free + align;
    request        = (request + align - 1) & ~(align - 1);
    ASSERT_LE(request, total_free);

    void * large = Allocate(&heap, request);
    ASSERT_EQ(large, nullptr);

    Deallocate(&heap, blocks[1]);
    large = Allocate(&heap, request);
    ASSERT_NE(large, nullptr);

    Deallocate(&heap, large);
    for(size_t i = 3; i < blocks.size(); ++i)
    {
        Deallocate(&heap, blocks[i]);
    }

    Deinit(&heap);
}

TEST(StaticHeapTest, SplitAndReuseRemainder)
{
    StaticHeap heap{};
    Init(&heap);

    void * a = Allocate(&heap, 64);
    ASSERT_NE(a, nullptr);
    Deallocate(&heap, a);

    void * b = Allocate(&heap, 24);
    void * c = Allocate(&heap, 24);
    ASSERT_NE(b, nullptr);
    ASSERT_NE(c, nullptr);

    Deallocate(&heap, b);
    Deallocate(&heap, c);
    Deinit(&heap);
}

TEST(StaticHeapTest, StressAllocateFreePattern)
{
    StaticHeap heap{};
    Init(&heap);

    std::vector<void *> slots(64, nullptr);
    std::vector<size_t> sizes(64, 0);
    uint32_t state = 0x12345678u;

    for(int i = 0; i < 1000; ++i)
    {
        state        = state * 1664525u + 1013904223u;
        size_t index = state % slots.size();

        if(slots[index])
        {
            Deallocate(&heap, slots[index]);
            slots[index] = nullptr;
            sizes[index] = 0;
            continue;
        }

        size_t size = 8u + (state % 32u);
        void * ptr  = Allocate(&heap, size);
        if(ptr)
        {
            memset(ptr, static_cast<int>(state & 0xFFu), size);
            slots[index] = ptr;
            sizes[index] = size;
        }
    }

    for(size_t i = 0; i < slots.size(); ++i)
    {
        if(slots[i])
        {
            Deallocate(&heap, slots[i]);
        }
    }

    ASSERT_FALSE(HasAdjacentFreeBlocks(&heap));

    size_t max_free = GetMaxFreeBlock(&heap);
    size_t align    = sizeof(std::max_align_t);
    size_t request  = (max_free / align) * align;
    if(request == 0)
    {
        request = max_free;
    }

    void * large = Allocate(&heap, request);
    ASSERT_NE(large, nullptr);
    Deallocate(&heap, large);
    Deinit(&heap);
}

TEST(DynamicHeapTest, StressAllocateFreePattern)
{
    DynamicHeap heap{};
    Init(&heap);

    std::vector<void *> slots(64, nullptr);
    std::vector<size_t> sizes(64, 0);
    uint32_t state = 0xA5A5A5A5u;

    for(int i = 0; i < 1000; ++i)
    {
        state        = state * 1103515245u + 12345u;
        size_t index = state % slots.size();

        if(slots[index])
        {
            Deallocate(&heap, slots[index]);
            slots[index] = nullptr;
            sizes[index] = 0;
            continue;
        }

        size_t size = 8u + (state % 32u);
        void * ptr  = Allocate(&heap, size);
        if(ptr)
        {
            memset(ptr, static_cast<int>(state & 0xFFu), size);
            slots[index] = ptr;
            sizes[index] = size;
        }
    }

    for(size_t i = 0; i < slots.size(); ++i)
    {
        if(slots[i])
        {
            Deallocate(&heap, slots[i]);
        }
    }

    void * large = Allocate(&heap, 200);
    ASSERT_NE(large, nullptr);
    Deallocate(&heap, large);
    Deinit(&heap);
}

TEST(DynamicHeapTest, CapacityLimitEnforced)
{
    DynamicHeap heap{};
    Init(&heap);

    std::vector<void *> blocks;
    size_t total       = 0;
    const size_t chunk = 24;

    while(true)
    {
        void * ptr = Allocate(&heap, chunk);
        if(!ptr)
        {
            break;
        }
        blocks.push_back(ptr);
        total += chunk;
    }

    ASSERT_FALSE(blocks.empty());

    void * extra = Allocate(&heap, chunk);
    ASSERT_EQ(extra, nullptr);

    for(void * ptr : blocks)
    {
        Deallocate(&heap, ptr);
    }

    void * after_free = Allocate(&heap, chunk);
    ASSERT_NE(after_free, nullptr);
    Deallocate(&heap, after_free);
    Deinit(&heap);
}

TEST(DynamicHeapTest, AllocationsStayInsideSinglePreallocatedBuffer)
{
    DynamicHeap heap{};
    Init(&heap);

    ASSERT_NE(heap.buffer, nullptr);
    ASSERT_NE(heap.head, nullptr);

    const uintptr_t begin = reinterpret_cast<uintptr_t>(heap.buffer);
    const uintptr_t end   = begin + heap.capacity;

    void * a = Allocate(&heap, 24);
    void * b = Allocate(&heap, 40);
    void * c = Allocate(&heap, 56);

    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    ASSERT_NE(c, nullptr);

    const uintptr_t pa = reinterpret_cast<uintptr_t>(a);
    const uintptr_t pb = reinterpret_cast<uintptr_t>(b);
    const uintptr_t pc = reinterpret_cast<uintptr_t>(c);

    ASSERT_GE(pa, begin);
    ASSERT_LT(pa, end);
    ASSERT_GE(pb, begin);
    ASSERT_LT(pb, end);
    ASSERT_GE(pc, begin);
    ASSERT_LT(pc, end);

    Deallocate(&heap, b);
    Deallocate(&heap, a);
    Deallocate(&heap, c);

    void * large = Allocate(&heap, 200);
    ASSERT_NE(large, nullptr);
    Deallocate(&heap, large);
    Deinit(&heap);
}

TEST(DynamicHeapTest, CallocateRespectsCapacity)
{
    DynamicHeap heap{};
    Init(&heap);

    void * a = Callocate(&heap, 4, 8);
    void * b = Callocate(&heap, 4, 8);
    void * c = Callocate(&heap, 4, 8);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    ASSERT_NE(c, nullptr);

    void * d = Callocate(&heap, 64, 8);
    ASSERT_EQ(d, nullptr);

    Deallocate(&heap, b);
    d = Callocate(&heap, 4, 8);
    ASSERT_NE(d, nullptr);

    Deallocate(&heap, a);
    Deallocate(&heap, c);
    Deallocate(&heap, d);
    Deinit(&heap);
}

TEST(DynamicHeapTest, ReallocateFailsUnderCapacityAndPreservesData)
{
    DynamicHeap heap{};
    Init(&heap);

    unsigned char * a = static_cast<unsigned char *>(Callocate(&heap, 4, 8));
    unsigned char * b = static_cast<unsigned char *>(Callocate(&heap, 4, 8));
    unsigned char * c = static_cast<unsigned char *>(Callocate(&heap, 4, 8));
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    ASSERT_NE(c, nullptr);

    for(size_t i = 0; i < 32; ++i)
    {
        a[i] = static_cast<unsigned char>(i);
    }

    void * resized = Reallocate(&heap, a, 192);
    ASSERT_EQ(resized, nullptr);
    for(size_t i = 0; i < 32; ++i)
    {
        ASSERT_EQ(a[i], static_cast<unsigned char>(i));
    }

    Deallocate(&heap, a);
    Deallocate(&heap, b);
    Deallocate(&heap, c);
    Deinit(&heap);
}
