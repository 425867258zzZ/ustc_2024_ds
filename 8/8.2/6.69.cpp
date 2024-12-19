#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

typedef struct TreeNode
{
    char str;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : str(x), left(nullptr), right(nullptr) {}
} Tree;

stack<char> str_s;
stack<int> space_s;

void IOT(Tree *tree, int floor)
{
    if (tree)
    {
        IOT(tree->left, floor + 1);
        str_s.push(tree->str);
        space_s.push(floor);
        IOT(tree->right, floor + 1);
    }
}

void PrintTree(Tree *tree)
{
    IOT(tree, 0);
    int space = space_s.top();
    char str = str_s.top();
    for (int i = 0; i < space; i++)
        cout << " " << "";
    cout << str << endl;
    space_s.pop();
    str_s.pop();
}
