#include <vector>
#define MAX_V 20
typedef int ElemType, Status;
typedef int GraphKind; // 定义图的类型，无向图0， 有向图1，无向网2，有向网3

// 定义边的结点结构类型
typedef struct ArcNode
{
    int adjvex;
    int weight;
    struct ArcNode *next;
} ArcNode;

// 定义顶点的结构类型
typedef struct VexNode
{
    ElemType data;
    ArcNode *arclist;
} VexNode;

class ALGraph
{
private:
    VexNode vnode[MAX_V];
    int vexnum, arcnum;
    GraphKind type;

public:
    bool IsTrans()
    {

        for (int ix = 0; ix < vexnum; ix++)
        {
            for (ArcNode *ptr_x = vnode[ix].arclist; ptr_x != nullptr; ptr_x = ptr_x->next)
            {
                int y = ptr_x->adjvex;
                for (ArcNode *ptr_y = vnode[y].arclist; ptr_y != nullptr && ptr_y->adjvex != ix; ptr_y = ptr_y->next)
                {
                    int z = ptr_y->adjvex;
                    int flag = 0;
                    for (ArcNode *__ptr_x = vnode[ix].arclist; __ptr_x != nullptr; __ptr_x = __ptr_x->next)
                    {
                        if (__ptr_x->adjvex == z)
                        {
                            flag = 1;
                            break;
                        }
                    }
                    if (flag)
                        return false;
                }
            }
        }
        return true;
    }
};