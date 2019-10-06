#include <iostream>
using namespace std;

int main(){
    char c[33] = "0123456789abcdefghijklmnopqrstu";
    void *p = c;
    char *p1;    /* 1 Byte */
    short *p2;   /* 2 Byte */
    int *p3;     /* 4 Byte */

    p1 = (char *)p;
    p2 = (short *)p;
    p3 = (int *)p;

    cout << "p1    : " << *p1     << endl;
    cout << "p2    : " << *p2     << endl;
    cout << "p3    : " << *p3     << endl;
    cout << "(p1+1): " << *(p1+1) << endl;
    cout << "(p2+1): " << *(p2+1) << endl; /* 23   */
    cout << "(p3+1): " << *(p3+1) << endl; /* 4567 */

    cout << "p1    : " << *p1     << endl;
    cout << "(p1+2): " << *(p1+2) << endl;
    cout << "(p1+4): " << *(p1+4) << endl;
    cout << "(p1+6): " << *(p1+6) << endl;

    short *q = p2 + 1;
    int *s = (int *)q;
    *s = 0;

    cout << "p1    : " << *p1     << endl;
    cout << "(p1+2): " << *(p1+2) << endl;
    cout << "(p1+4): " << *(p1+4) << endl;
    cout << "(p1+6): " << *(p1+6) << endl;

    return 0;
}

/* 
 *
 *     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u
 *    &0 &1 &2 &3 &4 &5 &6 &7 &8 &9 &a &b &c &d &e &f &g &h &i &j &k &l &m &n &o &p &q &r &s &t &u
 * p2: 0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15
 *  q:       v--v -> 23
 *  s:       v--------v -> 2345
 */