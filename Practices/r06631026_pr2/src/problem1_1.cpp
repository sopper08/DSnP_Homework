#include <iostream>
using namespace std;

/* function prototype */
// void f(int a);

int main(){
    f(0);
    return 0;
}

void f(int a){
    cout << a << endl;
}

/* error:
 *   problem1_1.cpp:8:8: error: ‘f’ was not declared in this scope
 *
 */