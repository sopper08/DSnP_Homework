#include <iostream>
using namespace std;

void f()
{
    int *p = new int(10);
    cout << p << endl;
    delete p;
    // p = 0;
    cout << p << endl;
    *p = 30;
    int j = *p;
    cout << j << endl;
    int *q = new int(20);
    cout << q << endl;
    int k = *p;
    cout << k << endl;
    *p = 40;
    cout << k << endl;
    cout << *q << endl;
}

int main(){
    f();
    return 0;
}