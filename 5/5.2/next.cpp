#include <iostream>
#include <list>

using namespace std;

int Next(string t, int j)
{
    for (int k = j - 2; k >= 0; k--)
    {
        if (t[k] == t[j - 1])
        {
            if (t.substr(0, k + 1) == t.substr(j - k - 1, k + 1))
            {
                return k + 1;
            }
        }
    }
    return 0;
}

int main()
{
    string t = "abcaabbabcab";
    list<int> next = {-1};
    for (int j = 1; j < static_cast<int>(t.length()); j++)
        next.push_back(Next(t, j));
    for (auto i : next)
        cout << i << " ";
    return 0;
}