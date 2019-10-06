#include <iostream>
using namespace std;

class B {
public:
    B(int i)
        :_bd(i) {}
    void printData(){
        cout << "Content of _bd: " << _bd << endl;
    }
private:
    int _bd;
};

class A {
public:
    A(int i)
        : _b(i){}
    B _b;

private:
    int _ad;
};

int main(){
    A a(1);
    a._b.printData();

    return 0;
}