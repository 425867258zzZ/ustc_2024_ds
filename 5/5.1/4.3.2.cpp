#include <iostream>
#include <string>

using namespace std;

string strReverse(string str)
{
    if (str.empty())
        return "";
    return strReverse(str.substr(1)) + str[0];
}

int main()
{
    cout << strReverse("1234567890") << endl;
}