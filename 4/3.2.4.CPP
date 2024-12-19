#include <iostream>

int ackerman(int m, int n)
{
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackerman(m - 1, 1);
    else
        return ackerman(m - 1, ackerman(m, n - 1));
}

int main()
{
    int m, n;
    std::cout << "Enter m and n: ";
    std::cin >> m >> n;
    std::cout << "ackerman(" << m << ", " << n << ") = " << ackerman(m, n) << std::endl;
    return 0;
}