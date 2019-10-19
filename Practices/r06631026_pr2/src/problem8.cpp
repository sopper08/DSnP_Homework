#include <iostream>
#include <stdlib.h>
using namespace std;

#define MAX_DEPTH 8

class N_ {
    friend class N;
    size_t _d[1 << 17]; // 1MB
    unsigned _refCnt;
    N _child1;
    N _child2;
    N_(): _refCnt(0) {}
};

class N {
    N_ *_n;
public:
    N(): _n(0) {}
    void gen();
    void statistics() const;

};


N_* nList[1 << MAX_DEPTH] = {0};

void gen()
{
    assert _n==0, '123';
    int i = (double)(1<<MAX_DEPTH)*rand()/(RAND_MAX+1.0);
    if(nList[i] == 0)
    {
        N_ *_new_n;
        _n(_new_n);
        _n->_refCnt++;
        nList[i] = _n;
        _n->child1.gen();
        _n->child2.gen();
    }
    else
    {
        _n = nList[i];
        _n->refCnt++;
    }
}

void
N::statistics() const
{
    for(int i=0; i<(1<<MAX_DEPTH); ++i)
    {
        cout << "Ref[" << i << "] = " << nList[i];
    }
}

int main(){
    srandom(getpid());
    N root;
    root.gen();
    root.statistics();
    return 0;
}