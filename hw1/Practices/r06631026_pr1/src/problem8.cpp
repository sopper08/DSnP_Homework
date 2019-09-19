#include <iostream>
using namespace std;

class C{
public:
    C(int _a, int _b)
        : _a(_a), _b(_b){}
    void printAddr() const {
        cout << "Address of this->_a: " << &this->_a << endl;
        cout << "Address of this->_b: " << &this->_b << endl;
    }
    void printData() const {
        cout << "Content of this->_a: " << this->_a << endl;
        cout << "Content of this->_b: " << this->_b << endl;
    }

private:
    int _a;
    int _b;
};

int main(){
    C c1(1,2);
    c1.printData();
    c1.printAddr();
    cout << "---" << endl;
    C c2(3,4);
    c2.printData();
    c2.printAddr();

    return 0;
}
