#include <algorithm>
#include <iostream>
#include <string.h>
#include <vector>
#define MVNum 100

using namespace std;
typedef struct AMGraph
{
    vector<char> v;
    int arcs[MVNum][MVNum];
    int vnum;
    int arcnum;
} AMGraph;

class Graph
{
private:
    AMGraph graph;

public:
    Graph()
    {
        memset(graph.arcs, 0, sizeof(int) * MVNum * MVNum);
        graph.vnum = graph.arcnum = 0;
    }

    int LocateV(char v);

    bool InsertVex(char v)
    {
        if (graph.vnum == MVNum)
        {
            std::cout << "点数到达上线" << std::endl;
            return false;
        }
        auto it = find(graph.v.begin(), graph.v.end(), v);
        if (it != graph.v.end())
        {
            std::cout << "点已存在" << std::endl;
            return false;
        }
        graph.v.push_back(v);
        return true;
    }

    bool InsertArc(char v, char w)
    {
        auto itv = find(graph.v.begin(), graph.v.end(), v);
        auto itw = find(graph.v.begin(), graph.v.end(), v);
        if (itv == graph.v.end() || itw == graph.v.end())
        {
            std::cout << "v or w 不存在" << std::endl;
            return false;
        }
        int iv = LocateV(v);
        int iw = LocateV(w);

        graph.arcs[iv][iw] = 1;
        return true;
    }

    bool DeleteVex(char v)
    {
        auto it = find(graph.v.begin(), graph.v.end(), v);
        if (it == graph.v.end())
        {
            std::cout << "点不存在" << std::endl;
            return false;
        }
        graph.v.erase(it);
        return true;
    }

    bool DeleteArc(char v, char w)
    {
        auto itv = find(graph.v.begin(), graph.v.end(), v);
        auto itw = find(graph.v.begin(), graph.v.end(), v);
        if (itv == graph.v.end() || itw == graph.v.end())
        {
            std::cout << "v or w 不存在" << std::endl;
            return false;
        }
        int iv = LocateV(v);
        int iw = LocateV(w);

        graph.arcs[iv][iw] = 0;
        return true;
    }
};
