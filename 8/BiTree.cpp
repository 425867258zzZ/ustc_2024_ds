#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

template <typename T>
struct BiTreeNode
{
    T data;
    BiTreeNode<T> *left;
    BiTreeNode<T> *right;
    BinTreeNode() : left(nullptr), right(nullptr) {}
    BiTreeNode(T x) : val(x), left(nullptr), right(nullptr) {}
};

template <typename T>
class BiTree
{
private:
    BinTreeNode<T> *root;

public:
};