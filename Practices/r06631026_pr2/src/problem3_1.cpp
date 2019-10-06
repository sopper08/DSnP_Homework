#include <iostream>
using namespace std;

class T{
public:
    T(int i): _d(i){};
    int _d;
    T& f(){ _d+=10; return (*this);}
    const void p(){ cout << _d << endl;}
};

int main(){
    const T a(10);
    a.f();
    return 0;
}

/* error: problem3_1.cpp:14:9: error: passing ‘const T’ as ‘this’ argument discards qualifiers */