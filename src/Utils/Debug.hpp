#include <cassert>
#include <iostream>

#ifndef NDEBUG
    #define LOG(x) std::cout << (x) << std::endl;
    #define ASSERT(x) assert((x));
#else
    #define LOG(x)
    #define ASSERT(x)
#endif