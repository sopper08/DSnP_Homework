#include <iostream>
using namespace std;

/* function prototype */
void f(int a=1);

int main(){
    f(0);
    return 0;
}

void f(int a=1){
    cout << a << endl;
}

/* error:
 *   problem1_2.cpp:12:15: error: default argument given for parameter 1 of ‘void f(int)’ [-fpermissive]
 */