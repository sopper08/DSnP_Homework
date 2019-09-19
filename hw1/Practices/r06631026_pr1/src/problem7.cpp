#include <iostream>
using namespace std;

class C{
public:
    void printAddr() const {
        cout << "Address of this->_a: " << &this->_a << endl;
        cout << "Address of this->_b: " << &this->_b << endl;
    }

private:
    int _a;
    int _b;
};

int main(){
    C c1;
    c1.printAddr();
    cout << "Address of c1: " << &c1 << endl;
    cout << "---" << endl;
    C c2;
    c2.printAddr();
    cout << "Address of c2: " << &c2 << endl;

    return 0;
}
