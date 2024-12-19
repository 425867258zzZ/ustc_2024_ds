#include <iostream>
#include <stack>

using namespace std;

int evalRNP(char *token)
{
    stack<int> stack;
    for (int i = 0; token[i] != '$'; i++)
    {
        if (isdigit(token[i]))
        {
            int num = int(token[i]);
            while (isdigit(token[i + 1]))
            {
                num = num * 10 + int(token[i]);
                i++;
            }
            stack.push(num);
        }
        else if (token[i] == '+')
        {
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(a + b);
        }
        else if (token[i] == '-')
        {
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(a - b);
        }
        else if (token[i] == '*')
        {
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(a * b);
        }
        else if (token[i] == '/')
        {
            int a = stack.top();
            stack.pop();
            int b = stack.top();
            stack.pop();
            stack.push(a / b);
        }
    }
    int res = stack.top();
    return res;
}

int main()
{
    char token[] = "1 2+$";
    cout << evalRNP(token) << endl;
    return 0;
}