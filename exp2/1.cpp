// n-皇后问题的随机搜索算法，找到解即可
// 用一维数组来存放每一列皇后的行号

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 采用堆分配存储的方式实现
int *solution; // 每列一个皇后，solution[col]=row,表示第col列的皇后在row行
int len;       // 皇后数量，len*len的棋盘
int *line1, *line2;

// 函数声明
void initSolution();
void restart();
void swap(int, int);
int traverseNeighbors();
int evaluate();

int main()
{
    clock_t start, end;
    double time;
    // long seed = 72;
    // 随机数种子，如果这个数不改变，那么每次算法运行的结果都是一样的，即使用rand()函数
    // srand(seed);  //这一句注释掉，等于每次的随机数种子都不一样

    // printf("请输入皇后个数：");
    // scanf("%d",&len);     	// 键盘读入棋盘的大小
    // printf("\n");
    len = 10000; // 棋盘大小
    solution = (int *)malloc(sizeof(int) * len);
    line1 = (int *)malloc(sizeof(int) * (2 * len - 1));
    line2 = (int *)malloc(sizeof(int) * (2 * len - 1)); // 下标为从0到2*len-2
    if ((!solution) || (!line1) || (!line2))
    {
        printf("分配空间失败");
        return 0; // 分配空间失败
    }

    start = clock();
    initSolution(); // 初始化一个棋盘布局

    int flag;
    while ((flag = traverseNeighbors()) != 0)
        if (flag < 0)
            restart();

    printf("冲突数为：%d\n得到的解为：", evaluate());
    for (int i = 0; i < len - 1; i++)
        printf("%d,", solution[i]);
    printf("%d", solution[len - 1]);
    end = clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\n用时：%lf", time);
    printf("（按下任意键结束）");
    getchar();

    free(solution);
    free(line1);
    free(line2);
    return 0;
}

/***  初始化一个棋盘布局,将邻居数组准备好  ***/
void initSolution()
{
    // 随机给初始布局或给定某个初始布局
    for (int i = 0; i < len; i++)
        solution[i] = i;
    for (int i = 0; i < 2 * len - 1; i++)
    { // 初始化
        line1[i] = 0;
        line2[i] = 0;
    }
    for (int i = 0; i < len; i++)
    {
        line1[i + solution[i]]++;
        line2[solution[i] - i + len - 1]++;
    }
    for (int i = 0; i < len; i++) // 对棋盘进行len次随机交换
        swap(i, rand() % len);
}

/***  将棋盘的第i列和第j列交换  ***/
void swap(int i, int j)
{
    int temp;
    line1[i + solution[i]]--;
    line2[solution[i] - i + len - 1]--;
    line1[j + solution[j]]--;
    line2[solution[j] - j + len - 1]--;

    temp = solution[j];
    solution[j] = solution[i];
    solution[i] = temp;

    line1[i + solution[i]]++;
    line2[solution[i] - i + len - 1]++;
    line1[j + solution[j]]++;
    line2[solution[j] - j + len - 1]++;
}

/***  计算棋盘的评价函数（棋盘的价值: value=可相互攻击到的皇后对数）
 ***  当皇后相互攻击不到时，目标函数的值value==0
 ***   solution[i]==solution[j],
      或者(solution[i]-solution[j])/(i-j)==1或-1时（C语言实现会有bug），
      两个皇后彼此能攻击到，value++
 ***  棋盘编码方式能保证不会出现 solution[i]==solution[j]，故该条件可以不用判断
 ***  思考：该函数的时间性能是否能提高？
 ***/
int evaluate()
{
    int value = 0;
    for (int i = 0; i < 2 * len - 1; i++)
    {
        if (line1[i])
            value += line1[i] - 1;
        if (line2[i])
            value += line2[i] - 1;
    }
    return value;
}

/***  检查当前解（棋盘）的邻居棋盘
     （交换solution[i]和solution[j],i和j随机选择，共有n(n-1)/2种不同的可能）
 ***  保留冲突最小的棋盘，将它视为新的解（best found策略）
 ***  若返回-1,表示没有找到更好的邻居，需要重启 restart() ,返回0表示找到所求的解，
 ***  返回大于0的值表示需要更新当前解solution后要继续遍历新解的邻居
 ***/
int localEvaluate(int i)
{
    return line1[i + solution[i]] + line2[solution[i] - i + len - 1] - 2;
}

int traverseNeighbors()
{
    int min_value = evaluate();     // 当前棋盘的所有邻居的最小评价函数值
    int min_col = -1, min_row = -1; // 上述对应的邻居

    // 遍历邻居，评估每个邻居冲突皇后对的个数，找到最小冲突对数的邻居
    int temp = min_value;
    int temp1, temp2;
    for (int i = 0; i < len; i++)
    {
        for (int j = i + 1; j < len; j++)
        {
            temp1 = localEvaluate(i) + localEvaluate(j); // 交换前
            swap(i, j);
            temp2 = localEvaluate(i) + localEvaluate(j); // 交换后
            if (temp1 > temp2)
            {
                temp = min_value - temp1 + temp2;
                min_col = i;
                min_row = j;
            }
            swap(i, j); // 交换回原状
        }
    }

    if (min_col == -1)
    {
        /*如果遍历了所有的邻居节点，也找不到更好的移动方式，
        那么就需要重新启动算法，初始棋局改变一下*/
        printf("找不到解，正在重新生成初始解和运行搜索算法中...\n");
        return -1;
    }
    swap(min_col, min_row); // 找到更好的移动方式，那么就以这个邻居进行下一次爬山
    return temp;
}

/***  当遍历完所有邻居，也没有找到更好的邻居
 ***  那么随机重置solution，然后再用爬山法搜索
 ***  随机重启：邻居是邻域内的“微小”的扰动，增加扰动范围，比如随机交换times=20次
 ***/
void restart()
{
    // 随机交换解的次数，扰动大小,设置times>1即可
    for (int i = 0; i < len; i++)
        swap(i, rand() % len);
    puts("restart()被调用！");
}
