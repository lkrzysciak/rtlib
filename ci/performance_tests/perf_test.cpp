#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include "containers/list.h"

int main()
{
    int samples[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    uint32_t buf[10000];
    List * list = List_Init(buf, sizeof(buf), sizeof(int));

     // Get starting timepoint 
    auto start = std::chrono::high_resolution_clock::now(); 
   
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            int temp_var = samples[j];
            List_PushBack(list, &temp_var);
        }
        for(int j=0; j<16; ++j)
        {
            List_PopBack(list);
        }
    }
  
    // Get ending timepoint 
    auto stop = std::chrono::high_resolution_clock::now(); 

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration c list: " << duration.count() << std::endl;

    // uint32_t buf[10000];
    // List * list = List_Init(buf, sizeof(buf), sizeof(int));

    std::list<int> temp_list{};

     // Get starting timepoint 
    start = std::chrono::high_resolution_clock::now(); 
   
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            temp_list.emplace_back(samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            temp_list.pop_back();
        }
    }
  
    // Get ending timepoint 
    stop = std::chrono::high_resolution_clock::now(); 

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration stl list: " << duration.count() << std::endl;

    std::vector<int> temp_vector{};

     // Get starting timepoint 
    start = std::chrono::high_resolution_clock::now(); 
   
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            temp_vector.emplace_back(samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            temp_vector.pop_back();
        }
    }
  
    // Get ending timepoint 
    stop = std::chrono::high_resolution_clock::now(); 

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration stl vector: " << duration.count() << std::endl;
    return 0;
}