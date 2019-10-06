#include <iostream>
#include "a.h"
#include "b.h"
using namespace std;

int main(){
    A a;
    B b;
    return 0;
}

// ./a.h:1:7: error: redefinition of ‘class A’
//  class A { };
//        ^
// In file included from ./problem5.cpp:2:0:
// ./a.h:1:7: error: previous definition of ‘class A’
//  class A { };