#include <iostream>
using namespace std;

void f(){
    int a, b, c;
    cout << "Address of a: " << &a << endl;
    cout << "Address of b: " << &b << endl;
    cout << "Address of c: " << &c << endl;
}

void f1(){
    int *p = new int;
    int *q = new int;
    cout << "Address of p: " << &p << endl;
    cout << "Address of q: " << &q << endl;
}

int main(){
    cout << "f(): " << endl;
    f();
    f();
    f();
    cout << "f1(): " << endl;
    f1();
    f1();
    return 0;
}