#include <iostream>
using namespace std;

class Base
{
public:
    virtual void f() { cout << "Base::f() is called" << endl; }
    void g() { cout << "Base::g() is called" << endl; }
    virtual void h() { cout << "Base::h() is called" << endl; }
};

class Derived: public Base
{
public:
    void f() { cout << "Derived::f() is called" << endl; }
    void g() { cout << "Derived::g() is called" << endl; }
};

int main(){
    Base *p = new Derived;
    Base *q = new Base;
    Derived *r = new Derived;
    p->f(); p->g(); p->h();
    q->f(); q->g(); q->h();
    r->f(); r->g(); r->h();

    return 0;
}
