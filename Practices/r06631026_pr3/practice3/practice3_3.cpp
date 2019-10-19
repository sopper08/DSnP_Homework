#include <iostream>
using namespace std;

class Base
{
public:
    virtual void f() { cout << "Base::f() is called" << endl; }
    void g() { cout << "Base::g() is called" << endl; }
    // virtual void h() { cout << "Base::h() is called" << endl; }
    virtual void h() = 0;
};

class Derived: public Base
{
public:
    void f() { cout << "Derived::f() is called" << endl; }
    void g() { cout << "Derived::g() is called" << endl; }
    // void h();
};

int main(){
    Base *p = new Derived;
    p->f(); p->g(); p->h();

    return 0;
}
