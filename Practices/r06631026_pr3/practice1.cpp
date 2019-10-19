#include <iostream>
using namespace std;

class Base
{
public:
    // virtual void f() { cout << "Base::f() is called" << endl; }
    void f() { cout << "Base::f() is called" << endl; }
    void g() { cout << "Base::g() is called" << endl; }
};

class Derived: public Base
{
public:
    void f() { cout << "Derived::f() is called" << endl; }
    void g() { cout << "Derived::g() is called" << endl; }
};

int main(){
    Base b;
    Derived d;
    b.f();
    b.g();
    d.f();
    d.g();  
    return 0;
}
