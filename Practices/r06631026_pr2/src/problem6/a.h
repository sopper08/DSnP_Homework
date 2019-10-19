#ifndef A_H
#define A_H

#include <iostream>
using namespace std;

// #define SET_LSB_MASK (size_t())
#define EDGE_BITS 1
#define PTR_MASK ((~(size_t(0)) >> EDGE_BITS) << EDGE_BITS)

class N
{
private:
    void *_p;

public:
    N(void* p): _p(p) { }
    void setMark()
    {
        _p = (void *)((size_t)_p | (~PTR_MASK));
    }
    bool checkMark() const 
    {
        return ((size_t)_p & (~PTR_MASK));
    }
    void* getPtr()
    {
        return (void*)((size_t)_p & PTR_MASK);
    }

};

#endif