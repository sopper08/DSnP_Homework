#include <iostream>

using namespace std;

class C;
class B{
public:
    int _bd;
    B(int i=0): _bd(i){}
};
class A{
public:
    B _b;
    C *_c;

    A(int i=0): _b(i), _c(0){}
};

int main(){
    A a(23);
    cout << "Address of a       : " << &a << endl;
    cout << "Address of a._b    : " << &a._b << endl;
    cout << "Address of a._b._bd: " << &a._b._bd << endl;
    cout << "Address of a._c    : " << &a._c << endl;
    cout << "Content of a._b._bd: " << a._b._bd << endl;
    cout << "Content of a._c    : " << a._c << endl;
}
