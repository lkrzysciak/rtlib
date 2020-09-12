#include <iostream>
#include <chrono>
#include "memory/object_pool.h"
#include "memory/typed_pool.h"

typed_pool_t(TestPool, int, 20);

void measureGenericPool()
{
    uint32_t pool_buf[10000];
    Pool* pool = Pool_Init(pool_buf, sizeof(pool_buf), sizeof(int));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            void* ptr = Pool_Alloc(pool);
            Pool_Free(pool, ptr);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[pool][rtlib][generic]: " << duration.count() << std::endl;
}

void measureTypedPool()
{
    TestPool test_pool{};
    TestPool_Init(&test_pool);

    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            int* ptr = TestPool_Alloc(&test_pool);
            TestPool_Free(&test_pool, ptr);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "[pool][rtlib][typed]: " << duration.count() << std::endl;
}

int main()
{
    measureGenericPool();
    measureTypedPool();

    return 0;
}