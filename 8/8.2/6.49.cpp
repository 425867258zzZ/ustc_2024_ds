#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

typedef struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
} Tree;

bool Judge(Tree *tree)
{
    if (tree == nullptr)
        return 0;
    queue<Tree *> t;
    t.push(tree);
    int cur_wide = 1;
    int flag = 0; // 标记树是否应该再无后继节点
    while (!t.empty())
    {
        for (int i = 0; i < static_cast<int>(t.size()); i++)
        {
            Tree *child = t.front();
            t.pop();
            if (flag)
            {
                if (child->left != nullptr || child->right != nullptr)
                    return false;
            }
            else if (child->left == nullptr && child->right != nullptr)
                return false;
            else if (child->left == nullptr && child->right != nullptr)
                flag = 1;
            else
            {
                t.push(child->left);
                t.push(child->right);
            }
        }
    }
    return true;
}