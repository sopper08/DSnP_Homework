#include <iostream>
#include "a.h"
using namespace std;

int main(){
    int a, b;
    N n1(&a);
    N n2(&b);

    cout << "-- Mark n1. --" << endl;
    n1.setMark();
    cout << "Mark of n1            : " << n1.checkMark() << endl;
    cout << "Mark of n2            : " << n2.checkMark() << endl;
    cout << "Address of a          : " << &a << endl;
    cout << "Address of b          : " << &b << endl;
    cout << "Address of n1.getPtr(): " << n1.getPtr()    << endl;
    cout << "Address of n2.getPtr(): " << n2.getPtr()    << endl;


    return 0;
}