#include <iostream>
using namespace std;

int main(){
    int a = 10;
    int &r = a;
    cout << "Address of a: " << &a << endl;
    cout << "Address of r: " << &r << endl;
    cout << "Content of a: " <<  a << endl;
    cout << "Content of r: " <<  r << endl;
    cout << "---" << endl;
    int b = 20;
    r = b;
    cout << "Address of a: " << &a << endl;
    cout << "Address of r: " << &r << endl;
    cout << "Address of b: " << &b << endl;
    cout << "Content of a: " <<  a << endl;
    cout << "Content of r: " <<  r << endl;
    cout << "Content of b: " <<  b << endl;
    
    return 0;
}