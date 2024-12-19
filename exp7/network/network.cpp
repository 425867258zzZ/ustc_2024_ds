#ifndef _NETWORK_CPP_
#define _NETWORK_CPP_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVERTEX 100 // 最大顶点数
#define MAXEDGE 20000 // 最大边数

// 顶点结构体
typedef struct Vertex
{
    int id;               // 顶点编号（唯一）
    int w;                // 顶点权值
    int degree;           // 顶点的度
    int edges[MAXVERTEX]; // 边的编号
} Vertex;

// 边结构体
typedef struct Edge
{
    int id; // 边的编号（唯一）
    int h;  // 边头顶点编号
    int t;  // 边尾顶点编号
    int w;  // 权值
} Edge;

// 图结构体
typedef struct Graph
{
    Vertex v[MAXVERTEX]; // 顶点数组
    Edge e[MAXEDGE];     // 边数组
    int nv;              // 顶点数
    int ne;              // 边数
    bool dirctional;     // t:有向图，f:无向图
    bool weighted;       // t:带权图，f:等权图
} Graph;

#include "showgt.h"

// 生成一个随机图，包括n个顶点，每个顶点和其它顶点连边的概率为p
int randgengraph(int n, float p, bool directional, bool weighted, const char *filename, int seed)
{
    srand(seed);
    char *content = (char *)malloc(5000000); // 存储顶点和边的信息
    if (!content)
    {
        printf("分配存储空间失败！\n");
        exit(0);
    }
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        printf("生成图文件%s时失败！\n", filename);
        exit(0);
    }
    long offset = 0; // content当前的写入位置
    int ne = 0;      // 生成边数
    for (int i = 0; i < n; i++)
        offset += sprintf(content + offset, "%3d %5d\n", i, rand() % 100); // 写入顶点的编号和顶点随机权值
    int pn = (int)(10000 * p);                                             // 每个顶点和其它顶点连边的概率
    if (directional)
    { // 有向图
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j && rand() % 10000 < pn)
                {
                    offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1); // 写入边的信息
                    ne++;
                }
    }
    else
    { // 无向图
        for (int i = 0; i < n; i++)
            for (int j = i; j < n; j++)
                if (i != j && rand() % 10000 < pn)
                {
                    offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1); // 写入边的信息
                    ne++;
                }
    }
    char ch[40]; // 存储图的基本信息
    int sz = sprintf(ch, "%d %7d %5s %5s\n", n, ne, weighted ? "true" : "flase", directional ? "true" : "flase");
    fwrite(&ch, sz, 1, fp);         // 写入图的顶点数和边数
    fwrite(content, offset, 1, fp); // 写入图的顶点和边的信息
    fclose(fp);
    free(content);
}

// 从文件中读入图，在内存中生成图的表示
void initGraphFromFile(Graph *g, const char *gfile)
{
    FILE *fp = fopen(gfile, "r");
    if (!fp)
    {
        printf("读取图数据文件出错！\n");
        exit(0);
    }
    char bv[10];
    // 读取图的基本信息
    fscanf(fp, "%u%u%s", &(g->nv), &(g->ne), bv);
    // printf("%d %d %s", g->ne, g->nv, bv);
    g->weighted = strcmp(bv, "true") == 0 ? true : false;
    fscanf(fp, "%s", bv); // 读取会覆盖之前的bv内容
    g->dirctional = strcmp(bv, "true") == 0 ? true : false;
    // 读取顶点信息
    for (int i = 0; i < g->nv; i++)
    {
        fscanf(fp, "%d%d", &(g->v[i].id), &(g->v[i].w));
        g->v[i].degree = 0;
        // printf("顶点%d-权%d\n", g->v[i].id, g->v[i].w);
    }
    // 读取边信息
    for (unsigned long i = 0; i < g->ne; i++)
    {
        fscanf(fp, "%u%u%d", &(g->e[i].h), &(g->e[i].t), &(g->e[i].w));
        // printf("边:%u-%u=>%d\n", g->e[i].h, g->e[i].t, g->e[i].w);
        g->e[i].id = i;
        unsigned h, t;
        h = g->e[i].h;
        t = g->e[i].t;
        g->v[h].edges[g->v[h].degree++] = i; // 将边id存入顶点h的边表
                                             // if(g->dirctional==false)
        g->v[t].edges[g->v[t].degree++] = i;
    }
    fclose(fp);
}

// 生成一个空图
Graph *initGraph(bool directional, bool weighted)
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->nv = g->ne = 0;
    g->dirctional = directional;
    g->weighted = weighted;
    return g;
}

int getEdgeIdx(Graph *g, int id); // 查找给定编号的边，返回其边数组下标

// 打印邻接表的信息
void printgraph(Graph *g)
{
    printf("图的基本信息:顶点数(%u)-边数(%lu)-%s-%s\n", g->nv, g->ne, g->weighted ? "加权图" : "等权图", g->dirctional ? "有向图" : "无向图");
    for (int i = 0; i < g->nv; i++)
    {
        printf("ID(%u)-度(%d)-权(%d)-边表 t(eid|w):%u", g->v[i].id, g->v[i].degree, g->v[i].w, g->v[i].id);
        for (int j = 0; j < g->v[i].degree; j++)
        {
            unsigned long e = getEdgeIdx(g, g->v[i].edges[j]);
            if (g->v[i].id == g->e[e].h)
                printf("->%u(%d)", g->e[e].t, g->e[e].w);
            else
                printf("->%u(%d)", g->e[e].h, g->e[e].w);
        }
        printf("\n");
    }
}

// 查找给定编号的顶点，返回其顶点数组下标
int getVexIdx(Graph *g, int id)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].id == id)
            return i;
    printf("input wrong vertex id in getVexIdx()!\n");
    exit(0);
}

// 查找给定编号的顶点，返回其权值
int getVexW(Graph *g, int id)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].id == id)
            return g->v[i].w;
    printf("input wrong vertex id in getVexW()!\n");
    exit(0);
}

// 查找给定编号的顶点，返回其度
int getVexDegree(Graph *g, int id)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].id == id)
            return g->v[i].degree;
    printf("input wrong vertex id in getVexDegree()!\n");
    exit(0);
}

// 查找给定编号的顶点,返回其所有边
int *getEdgesByNode(Graph *g, int id, int *ne)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].id == id)
        {
            *ne = g->v[i].degree;
            return g->v[i].edges;
        }
    printf("input wrong vertex id in getEdgesByNode()!\n");
    exit(0);
}

// 查找给定编号的边，返回其边数组下标
int getEdgeIdx(Graph *g, int id)
{
    for (int i = 0; i < g->ne; i++)
        if (g->e[i].id == id)
            return i;
    printf("input wrong edge id in getEdgeIdx()!\n");
    exit(0);
}

// 查找给定编号的边，返回其权值
int getEdgeW(Graph *g, int id)
{
    for (int i = 0; i < g->ne; i++)
        if (g->e[i].id == id)
            return g->e[i].w;
    printf("input wrong edge id in getEdgeW()!\n");
    exit(0);
}

// 查找给定编号的边，返回其两个顶点
void getVexByEdge(Graph *g, int id, int *h, int *t)
{
    for (int i = 0; i < g->ne; i++)
        if (g->e[i].id == id)
        {
            *h = g->e[i].h;
            *t = g->e[i].t;
            return;
        }
    printf("input wrong edge id in getVexByEdge()!\n");
    exit(0);
}

// 通过权值查找顶点编号
int locateVex(Graph *g, int w)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].w == w)
            return g->v[i].id;
    return -1;
}

// 通过权值查找边编号
int locateEdge(Graph *g, int w)
{
    for (int i = 0; i < g->ne; i++)
        if (g->e[i].w == w)
            return g->e[i].id;
    return -1;
}

// 设置顶点权值
void setVexW(Graph *g, int id, int w)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].id == id)
        {
            g->v[i].w = w;
            return;
        }

    printf("input wrong vertex id in setVexW()!\n");
    exit(0);
}

// 设置边权值
void setEdgeW(Graph *g, int id, int w)
{
    for (int i = 0; i < g->ne; i++)
        if (g->e[i].id == id)
        {
            g->e[i].w = w;
            return;
        }
    printf("input wrong edge id in setEdgeW()!\n");
    exit(0);
}

// 删除边
void deleteEdge(Graph *g, int id)
{
    int idx = getEdgeIdx(g, id);
    // 更新顶点的边信息
    for (int i = 0; i < g->nv; i++)
    {
        for (int j = 0; j < g->v[i].degree; j++)
        {
            if (g->v[i].edges[j] == id)
            {
                // 找到并删除顶点中的边编号
                for (int k = j; k < g->v[i].degree - 1; k++)
                {
                    g->v[i].edges[k] = g->v[i].edges[k + 1];
                }
                g->v[i].degree--; // 顶点度减1
                break;
            }
        }
    }

    // 删除
    for (int j = idx; j < g->ne - 1; j++)
    {
        g->e[j] = g->e[j + 1]; // 向左移位
    }
    g->ne--; // 边数减1

    return;
}

// 删除顶点，同时与其关联的边全部删除
void deleteVex(Graph *g, int id)
{
    // 删除顶点与其所有边

    int i = getVexIdx(g, id);

    // 删除与该顶点有关的所有边
    while (g->v[i].degree > 0)
    {
        deleteEdge(g, g->v[i].edges[0]);
    }
    // 删除顶点
    for (int j = i; j < g->nv - 1; j++)
    {
        g->v[j] = g->v[j + 1]; // 向左移位
    }
    g->nv--; // 顶点数减1
    return;

    printf("Vertex with ID %d not found.\n", id);
}

// 增加顶点
void addVex(Graph *g, int w)
{
    if (g->nv >= MAXVERTEX)
    {
        printf("Cannot add more vertices. Max limit reached.\n");
        return;
    }

    // 增加顶点
    int newIdx = g->nv;
    g->v[newIdx].id = g->v[newIdx - 1].id + 1; // 设置顶点的 ID
    g->v[newIdx].w = w;                        // 设置顶点的权值
    g->v[newIdx].degree = 0;                   // 初始度为 0

    g->nv++; // 顶点数加1
}

// 增加边
void addEdge(Graph *g, int w, int h, int t)
{
    if (g->ne >= MAXEDGE)
    {
        printf("Cannot add more edges. Max limit reached.\n");
        return;
    }

    int newIdx = g->ne;
    g->e[newIdx].id = g->e[newIdx - 1].id + 1; // 设置边的 ID
    g->e[newIdx].h = h;                        // 设置边的头顶点
    g->e[newIdx].t = t;                        // 设置边的尾顶点
    g->e[newIdx].w = w;                        // 设置边的权值

    int hidx = getVexIdx(g, h);
    g->v[hidx].edges[g->v[hidx].degree] = g->e[newIdx].id; // 将边的 ID 添加到头顶点的边列表中
    g->v[hidx].degree++;

    int tidx = getVexIdx(g, t);
    g->v[tidx].edges[g->v[tidx].degree] = g->e[newIdx].id; // 将边的 ID 添加到尾顶点的边列表中
    g->v[tidx].degree++;

    g->ne++; // 边数加1
}

// 声明访问标记数组
bool visited[MAXVERTEX];

// 深度优先搜索
void DFS(Graph *g, int v_id)
{
    // 标记当前顶点为已访问
    visited[v_id] = true;
    // 输出当前顶点的 id
    printf("%c ", 'a' + g->v[v_id].id);

    // 遍历该顶点的所有邻接顶点
    for (int i = 0; i < g->v[v_id].degree; i++)
    {
        int edge_id = g->v[v_id].edges[i];
        int neighbor_id;

        if (g->dirctional)
        { // 有向图
            if (g->e[edge_id].t == v_id)
            {
                neighbor_id = g->e[edge_id].h;
            }
            else
            {
                continue;
            }
            // 如果该邻接顶点没有被访问过，递归调用 DFS
            if (!visited[neighbor_id])
            {
                DFS(g, neighbor_id);
            }
        }
        else
        {
            // 获取边的另一个顶点
            if (g->e[edge_id].h == v_id)
            {
                neighbor_id = g->e[edge_id].t;
            }
            else
            {
                neighbor_id = g->e[edge_id].h;
            }

            // 如果该邻接顶点没有被访问过，递归调用 DFS
            if (!visited[neighbor_id])
            {
                DFS(g, neighbor_id);
            }
        }
    }
}

int main()
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    if (!g)
    {
        printf("error\n");
        exit(0);
    }
    // randgengraph(10, 0.2, 1, 1, "graph.txt", 5);
    initGraphFromFile(g, "graph.txt");
    /*     printgraph(g);
        int *h;
        int *t;
        addVex(g, 7);
        deleteVex(g, 0);
        addEdge(g, 1, 1, 7);
        deleteEdge(g, 7);
        int i = getVexIdx(g, 2);
        printgraph(g);
        for (int j = 0; j < g->v[i].degree; j++)
        {
            unsigned long e = getEdgeIdx(g, g->v[i].edges[j]);
            if (g->v[i].id == g->e[e].h)
                printf("->%u(%d)", g->e[e].t, g->e[e].w);
            else
                printf("->%u(%d)", g->e[e].h, g->e[e].w);
        }
        printf("\n");
        printgraph(g);
        setEdgeW(g, 9, 5);
        memset(visited, false, sizeof(bool) * MAXVERTEX); */
    DFS(g, 0);

    // printgraph(g);
    saveGraph(g, "net.html");
}
#endif