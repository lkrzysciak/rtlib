#include <iostream>
#include <chrono>
#include <list>
#include <vector>
#include "containers/list.h"
#include "containers/vector.h"
#include <memory_resource>

/*
template <typename T, size_t size, size_t alignment = sizeof(T)>
class MyAlloc {
    public:
    // type definitions
    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

    // rebind allocator to type U
    template <class U>
    struct rebind {
        typedef MyAlloc<U> other;
    };

    // return address of values
    pointer address (reference value) const {
        return &value;
    }
    const_pointer address (const_reference value) const {
        return &value;
    }

    MyAlloc() throw() {
    }
    MyAlloc(const MyAlloc&) throw() {
    }
    template <class U>
        MyAlloc (const MyAlloc<U>&) throw() {
    }
    ~MyAlloc() throw() {
    }

    // return maximum number of elements that can be allocated
    size_type max_size () const throw() {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    // allocate but don't initialize num elements of type T
    pointer allocate (size_type num, const void* = 0) {
        // print message and allocate memory with global new
        std::cerr << "allocate " << num << " element(s)"
                    << " of size " << sizeof(T) << std::endl;
        pointer ret = (pointer)(::operator new(num*sizeof(T)));
        std::cerr << " allocated at: " << (void*)ret << std::endl;
        return ret;
    }

    // initialize elements of allocated storage p with value value
    void construct (pointer p, const T& value) {
        // initialize memory with placement new
        new((void*)p)T(value);
    }

    // destroy elements of initialized storage p
    void destroy (pointer p) {
        // destroy objects by calling their destructor
        p->~T();
    }

    // deallocate storage p of deleted elements
    void deallocate (pointer p, size_type num) {
        // print message and deallocate memory with global delete
        std::cerr << "deallocate " << num << " element(s)"
                    << " of size " << sizeof(T)
                    << " at: " << (void*)p << std::endl;
        ::operator delete((void*)p);
    }
};

// return that all specializations of this allocator are interchangeable
template <class T1, class T2>
bool operator== (const MyAlloc<T1>&,
                const MyAlloc<T2>&) throw() {
    return true;
}
template <class T1, class T2>
bool operator!= (const MyAlloc<T1>&,
                const MyAlloc<T2>&) throw() {
    return false;
}
*/
int main()
{

    std::cout << sizeof(std::list<int>) << std::endl;
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

    std::fill_n(std::begin(buf), std::size(buf)-1, '_');

    std::pmr::monotonic_buffer_resource pool{
        std::data(buf), std::size(buf)
    };


    std::pmr::list<int> temp_list_mb{&pool};

     // Get starting timepoint 
    start = std::chrono::high_resolution_clock::now(); 
   
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            temp_list_mb.emplace_back(samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            temp_list_mb.pop_back();
        }
    }
  
    // Get ending timepoint 
    stop = std::chrono::high_resolution_clock::now(); 

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration stl list monotonic buffer: " << duration.count() << std::endl;


    uint32_t buf1[10000];
    Vector * vector = Vector_Init(buf1, sizeof(buf1), sizeof(int));

     // Get starting timepoint 
    start = std::chrono::high_resolution_clock::now(); 
   
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            // int temp_var = samples[j];
            Vector_PushBack(vector, &samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            Vector_PopBack(vector);
        }
    }
  
    // Get ending timepoint 
    stop = std::chrono::high_resolution_clock::now(); 

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration c vector: " << duration.count() << std::endl;



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

    std::fill_n(std::begin(buf), std::size(buf)-1, '_');

    std::pmr::monotonic_buffer_resource pool1{
        std::data(buf), std::size(buf)
    };
    std::pmr::vector<int> temp_vector_mb{&pool1};

     // Get starting timepoint 
    start = std::chrono::high_resolution_clock::now(); 
   
    for(int i=0; i<10000000; ++i)
    {
        for(int j=0; j<16; ++j)
        {
            temp_vector_mb.emplace_back(samples[j]);
        }
        for(int j=0; j<16; ++j)
        {
            temp_vector_mb.pop_back();
        }
    }
  
    // Get ending timepoint 
    stop = std::chrono::high_resolution_clock::now(); 

    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "duration stl vector monotonic buffer: " << duration.count() << std::endl;

    return 0;
}