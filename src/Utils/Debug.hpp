#include <cassert>
#include <iostream>

#ifndef NDEBUG
    #define LOG(x) std::cout    \
        << "[" << __FILE__      \
        << ":" << __func__      \
        << ":" << __LINE__      \
        << "] " << (x)          \
        << std::endl;
    #define ASSERT(x) assert((x));
#else
    #define LOG(x)
    #define ASSERT(x)
#endif