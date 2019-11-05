#include <iostream>
using namespace std;

class A
{
	char c;
};
int main()
{
    A* a = new A[74];
    void* v = a;
    size_t* s = (size_t*) v;
    cout << *(--s) << endl;
}

