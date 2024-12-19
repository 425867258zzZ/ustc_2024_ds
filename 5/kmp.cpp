#include <iostream>
#include <string>

using namespace std;

// 求得j对应的next值
int Next(string t, int j)
{
    if (j == 0)
        return -1;
    for (int k = j - 2; k >= 0; k--)
    {
        if (t[k] == t[j - 1])
        {
            if (t.substr(0, k + 1) == t.substr(j - k - 1, k + 1))
                return k + 1;
        }
    }
    return 0;
}

// 使用next数组
void SetNextArray(string t, int next[])
{
    for (int j = 0; j < t.size(); j++)
    {
        next[j] = Next(t, j);
    }
}

// 计算修正值nextval
int NextVal(string t, int j)
{
    int next[t.size()];
    SetNextArray(t, next);
    for (int j = 0; j < t.size(); j++)
    {
        int k = next[j];
        if (t[j] == t[k])
            next[j] = next[k];
    }
    return next[j];
}

int KMPIndex(string s, string t)
{
    int i, j;
    i = 0, j = 0;
    while ((i <= s.size() - t.size()) && j < static_cast<int>(t.size()))
    {
        if (j == -1 || s[i] == t[j])
        {
            ++j;
            ++i;
        }
        else
            j = NextVal(t, j);
    }
    return j >= t.size() ? i - t.size() : -1;
}
int main()
{
    string s = "hello";
    string t = "ll";
    cout << KMPIndex(s, t) << endl;
}