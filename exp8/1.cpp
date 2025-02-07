#include <cstring>
#include <iostream>

#define MAX 9999
int chosen[9999];

bool select(int n, int num, int m, int idx, int k)
{
    if (n < 0 || idx > k)
        return false;
    if (n == 0 && idx == 9)
        return true;
    for (int i = num + 1; i <= m; i++)
    {
        if (!chosen[i])
        {
            chosen[i] = 1;
            if (select(n - i, i, m, idx + 1, k))
            {
                std::cout << i;
                if (idx == 0)
                    std::cout << std ::endl;
                return true;
            }
            chosen[i] = 0;
        }
    }
    return false;
}

int main()
{
    int m = 10, k = 4, n = 18;
    memset(chosen, 0, 10 * sizeof(int));
    select(n, 0, m, 0, k);
}