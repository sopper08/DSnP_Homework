#include <iostream>
using namespace std;

class C{
public:
    double x;
    double y;
    double z;
};

int main(){
    C c;
    cout << "Size of C: " << sizeof(c) << endl;
    cout << "Address of c.x: " << &c.x << endl;
    cout << "Address of c.y: " << &c.y << endl;
    cout << "Address of c.z: " << &c.z << endl;
    cout << "---" << endl;
    int *a1 = new int;
    int *a2 = new int;
    C *c1 = new C;
    C *c2 = new C;
    C *c3 = new C;
    cout << "Address of a1: " << &a1 << endl;
    cout << "Address of a2: " << &a2 << endl;
    cout << "Address of c1: " << &c1 << endl;
    cout << "Address of c2: " << &c2 << endl;
    cout << "Address of c3: " << &c3 << endl;
    cout << "---" << endl;
    cout << "Address of c1->x: " << &c1->x <<endl;
    cout << "Address of c1->y: " << &c1->y <<endl;
    cout << "Address of c1->z: " << &c1->z <<endl;
    cout << "Address of c2->x: " << &c2->x <<endl;
    cout << "Address of c2->y: " << &c2->y <<endl;
    cout << "Address of c2->z: " << &c2->z <<endl;
    cout << "Address of c3->x: " << &c3->x <<endl;
    cout << "Address of c3->y: " << &c3->y <<endl;
    cout << "Address of c3->z: " << &c3->z <<endl;

    return 0;
}