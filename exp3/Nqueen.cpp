#include <chrono>
#include <cstdarg>
#include <iostream>
#include <random>
#include <string.h>

#define N 1000

int *pos;
int pdiag[2 * N + 1]; // 45°对角线上的元素记录，左上到右下为0到2n的索引
int ndiag[2 * N + 1]; // 135°对角线上的元素记录，左下到右上为0到2n的索引

#define get_pdiag(idx, row) N - (row - idx)

#define get_ndiag(idx, row) row + idx - 1

void swap(int i, int j)
{
    pdiag[get_pdiag(i, pos[i])]--;
    ndiag[get_ndiag(i, pos[i])]--;
    pdiag[get_pdiag(j, pos[j])]--;
    ndiag[get_ndiag(j, pos[j])]--;
    pdiag[get_pdiag(i, pos[j])]++;
    ndiag[get_ndiag(i, pos[j])]++;
    pdiag[get_pdiag(j, pos[i])]++;
    ndiag[get_ndiag(j, pos[i])]++;
    int t = pos[i];
    pos[i] = pos[j];
    pos[j] = t;
}

// 初始化棋盘
void InitBoard()
{
    for (int i = 1; i <= N; i++)
    {
        pos[i - 1] = i;
    }
    for (int j = 0; j <= N - 2; j++)
    {
        int idx = j + 1 + rand() % (N - (j + 1));
        int t = pos[j];
        pos[j] = pos[idx];
        pos[idx] = t;
    }
    memset(pdiag, 0, (2 * N + 1) * sizeof(int));
    memset(ndiag, 0, (2 * N + 1) * sizeof(int));
    for (int i = 0; i < N; i++)
    {
        pdiag[get_pdiag(i, pos[i])]++;
        ndiag[get_ndiag(i, pos[i])]++;
    }
}

// 评估当前棋盘分数（可相互攻击的皇后对数）
int Mark()
{
    int mark = 0;
    for (int j = 0; j < 2 * N + 1; j++)
    {
        if (pdiag[j] > 1)
            mark += pdiag[j] - 1;
        if (ndiag[j] > 1)
            mark += ndiag[j] - 1;
    }
    return mark;
}

int lineMark(int i, int row)
{
    int mark = 0;
    int p = get_pdiag(i, row);
    int n = get_ndiag(i, row);
    if (pdiag[p] > 1)
        mark += pdiag[p] - 1;
    if (ndiag[n] > 1)
        mark += ndiag[n] - 1;
    return mark;
}

int TraverseNeighbors()
{
    int min_mark = Mark();
    if (min_mark == 0)
    {
        return 0;
    } // 当前棋盘的所有邻居的最小评价函数值
    int flag = 0; // 上述对应的邻居
    for (int i = 0; i < N - 1; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            int tmp1 = lineMark(i, pos[i]) + lineMark(j, pos[j]) + lineMark(i, pos[j]) + lineMark(j, pos[i]);
            swap(i, j);
            int tmp2 = lineMark(i, pos[i]) + lineMark(j, pos[j]) + lineMark(i, pos[j]) + lineMark(j, pos[i]);
            int cur_mark = min_mark + tmp2 - tmp1;
            if (cur_mark == 0)
            {
                min_mark = cur_mark;
                flag = 1;
                break;
            }
            else if (cur_mark < min_mark)
            {
                min_mark = cur_mark;
                flag = 1;
            }
            else
            {
                swap(i, j);
            }
        }
        if (min_mark == 0)
            break;
    }

    if (!flag)
    {
        printf("找不到解，正在重新生成初始解和运行搜索算法中...\n");
        return -1;
    }
    return min_mark;
}

// 添加扰动重新开始
void restart()
{
    int times = 5; // 随机交换解的次数，扰动大小,设置times>1即可
    for (int i = 0; i < times; ++i)
        swap(rand() % N, rand() % N);
    puts("restart()被调用！");
}

int main()
{
    long seed = 4676676;
    srand(seed);
    auto start = std::chrono::system_clock::now();

    pos = (int *)malloc(sizeof(int) * N);
    if (!pos)
        return 0; // 分配空间失败

    InitBoard(); // 初始化一个棋盘布局

    int flag;
    while ((flag = TraverseNeighbors()) != 0)
        if (flag < 0)
            restart();

    printf("冲突数为：%d\n得到的解为：", Mark());

    free(pos);

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "程序运行时间：" << duration.count() << " ms" << std::endl;
}