#include <iostream>
using namespace std;

int main(){
    int a = 10;
    cout << "Address of a: " << &a << endl;
    int arr[10] = {0};
    cout << "Address of arr: " << &arr << endl;
    int *arrP[10];
    for(int i=0; i<10; ++i){
        arrP[i] = &arr[i];
        cout << "Address of arrP[" << i << "]: " << &arrP[i] << endl;
    }
    int *p1 = new int(10);
    int *p2 = new int[10];
    for(int i=0; i<10; ++i)
        cout << "Address of &p2[" << i << "]: " << &p2[i] << endl;

    int **p3 = new int*;
    *p3 = new int(20);
    int **p4 = new int*[10];
    for(int i=0; i<10; ++i)
        p4[i] = new int(i+2);
    int **p5 = new int*[10];
    for(int i=0; i<10; ++i){
        p5[i] = new int[i+2];
        cout << "Address of &p5[" << i << "]: " << &p5[i] << endl;
    }

    return 0;
}