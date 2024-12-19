#include <iostream>
#include <string>

using namespace std;

string format(string s1, int n, string &s3, string s2 = string())
{
    if (n <= 0 || s1.length() < static_cast<__SIZE_TYPE__> (n))
    {
        s3 = s1;
        return string();
    }
    int index;
    for (index = 0; s1[index] == '0' || s1[index + n - 1] == '0'; index++)
        ;
    s2 = s1.substr(index, n);
    s3 = s1.substr(0, index) + s1.substr(index + n);
    return s2;
}

int main()
{
    string s3 = "";
    cout << format("abcdefg", 5, s3) << endl;
}