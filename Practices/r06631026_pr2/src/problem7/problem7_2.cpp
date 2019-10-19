#include <iostream>
using namespace std;

int main(){
    enum COLOR
    {
        RED,
        BLUE,
        GREEN = 5
    };
    enum COLOR color = RED;
    cout << color << endl;

    return 0;
}