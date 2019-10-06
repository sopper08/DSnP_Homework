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
    const_cast<T *>(&a)->f().p();
    
    return 0;
}