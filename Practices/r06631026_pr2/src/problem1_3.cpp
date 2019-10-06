#include <iostream>
using namespace std;

/* function prototype */
void f(int a=2);

int main(){
    f();
    return 0;
}

void f(int a){
    cout << a << endl;
}