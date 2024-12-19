#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

typedef struct CSNode
{
    int data;
    struct CSNode *firstChild, *nextSibling;
} CSNode, *CSTree;

int CSleaf(CSNode *cstree)
{
    if (cstree == nullptr)
        return 0;
    if (cstree->firstChild == nullptr)
        return 1 + CSleaf(cstree->nextSibling);
    else
        return CSleaf(cstree->firstChild) + CSleaf(cstree->nextSibling);
}