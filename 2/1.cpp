#include <iostream>
using namespace std;
int main()
{
    int a, b, c, t;
    scanf("%d,%d,%d", &a, &b, &c);
    if (a > b)
    {
        t = a;
        a = b;
        b = t;
    }
    if (b > c)
    {
        t = b;
        b = c;
        c = t;
    }
    if (a > c)
    {
        t = a;
        a = c;
        c = t;
    }
    cout << c << b << a << "," << c << a << b << "," << b << a << c << "," << b << c << a << "," << c << a << b << "," << c << b << a;
}