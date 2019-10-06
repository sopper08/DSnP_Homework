#include <iostream>
using namespace std;

int main(){

    int a, b, c;
    cout << "&a" << &a << endl;
    cout << "&b" << &b << endl;
    cout << "&c" << &c << endl;
    cout << "a:" << a << endl;
    cout << "b:" << b << endl;
    cout << "c:" << c << endl;

    while(cin >> a >> b){
        c = a + b;
        cout << "&a" << &a << endl;
        cout << "&b" << &b << endl;
        cout << "&c" << &c << endl;
        cout << "a:" << a << endl;
        cout << "b:" << b << endl;
        cout << "c:" << c << endl;
    }

    return 0;
}
