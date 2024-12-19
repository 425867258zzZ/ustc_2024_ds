#ifndef HUFFMANTREE_CPP_
#define HUFFMANTREE_CPP_

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <vector>

using namespace std;

// 哈夫曼编码表
class HFzip
{
private:
    typedef struct HTNode
    {
        int id;          // 节点id
        long int weight; // 权值
        HTNode *parent, *lchild, *rchild;

        HTNode() : id(0), weight(0), parent(nullptr), lchild(nullptr), rchild(nullptr) {};
        HTNode(int id, long int weight) : id(id), weight(weight), parent(nullptr), lchild(nullptr), rchild(nullptr) {};
    } HTNode; // 三叉储存链表哈夫曼树

    typedef struct
    {
        unsigned int code; // 前面都是0，最后len位是编码
        int len;           // 编码长度
    } HCode;

    const int MAXLEN = 30;
    const int LEAF = 256;
    const int NODE = 511;
    HTNode *_hftree = nullptr; // 哈夫曼树
    HCode *_hfcode = nullptr;  // 哈夫曼编码表数组

public:
    /// TODO
    ~HFzip() = default;

    /// @brief 生成哈夫曼树，以三叉链表储存
    /// @param w 字符：权值数组
    /// @return 根节点指针
    HTNode *getHFTree(const long *w) const
    {
        vector<HTNode *> htnptr;
        int un_zero = 0;
        int maxw = 0;
        for (int i = 0; i < LEAF; ++i)
        {
            if (w[i] != 0)
            {
                auto *htn = new HTNode(i, w[i]);
                htnptr.push_back(htn);
                un_zero++;
                maxw += w[i];
            }
        }
        auto *root = new HTNode(-1, 0);
        HTNode *h1, *h2; // 记录最小值和次小值的指针
        for (int i = 0; i < un_zero - 1; i++)
        {
            int min2;
            int min1 = min2 = maxw + 1; // 初始化最小值
            auto it = htnptr.begin();
            while (it != htnptr.end())
            {
                if ((*it)->weight < min1)
                {
                    min2 = min1;
                    min1 = (*it)->weight;
                    h2 = h1;
                    h1 = *it;
                }
                else if ((*it)->weight < min2)
                {
                    min2 = (*it)->weight;
                    h2 = *it;
                }
                ++it;
            }

            // 设置新节点的权值和左右孩子，更新htnptr
            auto *htn = new HTNode(-i - 1, h1->weight + h2->weight); //-i-1目的为每一个node设置一个独特标识id，避免重复
            htn->lchild = h1;
            htn->rchild = h2;
            (h1)->parent = htn;
            (h2)->parent = htn;
            htnptr.erase(find(htnptr.begin(), htnptr.end(), h1));
            htnptr.erase(find(htnptr.begin(), htnptr.end(), h2));
            htnptr.push_back(htn);
            root = htn;
        }
        return root; // 返回根节点指针
    }

    // 树状图输出三叉树
    static void printTree(const HTNode *root)
    {
        static int level = -1; // 记录是第几层次

        if (!root)
            return;

        level++;
        printTree(root->rchild);
        level--;

        level++;
        for (int i = 0; i < level; i++)
            printf("\t");
        printf("%d\n", root->id);
        printTree(root->lchild);
        level--;
    }

    /*** 以下代码分析文件和从文件中生成哈夫曼编码表 ***/

    /// @brief 从filename中二进制读取字符，统计每个字符出现的次数，写入w并返回
    /// @param filename 待读取文件名
    /// @param w 字符：权值数组
    /// @param fsize 文件大小
    /// @return unsigned char *文件数据dataArray
    unsigned char *parseFile(const char filename[], long *w, long *fsize) const
    {
        FILE *fp = fopen(filename, "rb");
        if (fp == nullptr)
        {
            printf("无法打开文件!\n");
            exit(0);
        }
        fseek(fp, 0, SEEK_END);
        *fsize = ftell(fp);
        rewind(fp);
        auto *dataArray = static_cast<unsigned char *>(malloc(sizeof(unsigned char) * (*fsize)));
        if (!dataArray)
        {
            printf("文件太大，内存不够，读入错误!\n");
            exit(0);
        }
        fread(dataArray, sizeof(unsigned char), *fsize, fp); // 读取待压缩文件
        fclose(fp);

        memset(w, 0, LEAF * (sizeof(long)));

        for (int i = 0; i < *fsize; ++i) // 权值+1，若对应字符出现
            w[dataArray[i]]++;
        return dataArray;
    }

    // 依据id值c在树中查找节点
    static HTNode *getNode(HTNode *root, int c)
    {
        if (root != nullptr)
        {
            if (root->id == c)
                return root;
            if (HTNode *node = getNode(root->lchild, c))
                return node;
            return getNode(root->rchild, c);
        }
        return nullptr;
    }

    static void getCode(HTNode *root, int c, unsigned *code1, int *clen1)
    {
        int clen = 0;      // 编码长度
        unsigned code = 0; // 编码

        int size = sizeof(code);             // 编码的最大bits数 32位
        unsigned mask = 1 << (size * 8 - 1); // 最高位为1其它为0，用来设置最高位为1

        HTNode *node = getNode(root, c);
        if (node != nullptr)
        {
            HTNode *parent = node->parent;
            while (parent)
            {
                code = code >> 1;
                parent->lchild->id == c ? code : code |= mask; // 如果是右孩子，那么最高位设置为1，否则就是0
                clen++;
                c = parent->id;
                parent = parent->parent;
            }
        }

        // code的前面clen位是哈夫曼编码，将其移动到最后
        code = code >> (size * 8 - clen);
        // printf("0x%x-%d\n",code,clen);
        *code1 = code; // 返回值
        *clen1 = clen; // 返回值
    }

    // 从哈夫曼树ht生成完整的编码表hc，输出hc为encode()的输入参数
    void genHCodes(const long *w) const
    {
        for (int i = 0; i < LEAF; ++i)
            if (w[i] > 0)
                getCode(_hftree, i, &(_hfcode[i].code), &(_hfcode[i].len));
    }

    /*** 以下为编解码实现 ***/
    // 编码,原长olen的内容orgi，压缩编码后为长nlen的内容newc，需要利用编码表_hfcode
    void encode(const unsigned char *orgi, const long olen, unsigned char *newc, long *nlen) const
    {
        long i = 0; // orgi的下标
        long j = 0; // newc的下标
        newc[j] = 0;
        int usedbits = 0; // newc[j]中被占用的bits数
        while (i < olen)
        {
            // 对原始数据逐个字符处理
            const int idx = orgi[i]; // 获得hc下标
            unsigned code = _hfcode[idx].code;
            int len = _hfcode[idx].len;
            while (len + usedbits >= 8)
            {
                // code需要左移usedbits长的一段，然后和newc[j]位或操作
                unsigned code1 = code >> (len + usedbits - 8);
                newc[j] = newc[j] | code1; // 填满前一个编码字符
                len = len + usedbits - 8;  // code现在有效长度
                usedbits = 0;
                newc[++j] = 0;
                // printf("\n编码的第 %d 个字符为：#%2x\n",j-1,newc[j-1]);
            } // 循环，使得len+usedbits <8
            // 将code的前面各位清0,因为有效位是len
            code = code & ((1 << len) - 1);      // code和后面k个1，其它全为0的数据进行与，保留后面len位
            code = code << (8 - len - usedbits); // 向前移动若干位
            newc[j] = newc[j] | code;
            usedbits = len + usedbits;
            i++;
        }
        *nlen = j + 1;
    }

    // 获取哈夫曼树节点数
    static long getsize(const HTNode *root)
    {
        if (root)
        {
            return 1 + getsize(root->lchild) + getsize(root->rchild);
        }
        return 0;
    }

    // 中序存储到数组htnArray
    static void mid_hftree(const HTNode *root, int &i, HTNode midArray[])
    {
        if (root)
        {
            mid_hftree(root->lchild, i, midArray);
            i++;
            midArray[i] = *root;
            i++;
            mid_hftree(root->rchild, i, midArray);
        }
        else
            i--;
    }

    // 先序储存到数组htnArray
    static void fst_hftree(const HTNode *root, int &i, HTNode firArray[])
    {
        if (root)
        {
            firArray[i] = *root;
            i++;
            fst_hftree(root->lchild, i, firArray);
            i++;
            fst_hftree(root->rchild, i, firArray);
        }
        else
            i--;
    }

    static HTNode *getTree(HTNode *fir, int pl, int pu, HTNode *mid, int il, int iu)
    {
        int iu1, il2, iu2, pl1, pl2, pu1, pu2;
        int il1 = iu1 = il2 = iu2 = pl1 = pl2 = pu1 = pu2 = 0; // 根节点把中序序列分成两个子序列，同样得到先徐序列的两个子序列，这是子序列的下标
        if (pu - pl != iu - il)
            exit(0); // 检查输入参数
        if (pu - pl < 0)
            return nullptr;                               // 若序列为空，返回空结点/空树
        auto *t = new HTNode(fir[pl].id, fir[pl].weight); // 生成根结点
        for (int i = il; i <= iu; ++i)
        {
            if (mid[i].id == fir[pl].id)
            {
                iu1 = i - 1;
                il1 = il;
                il2 = i + 1;
                iu2 = iu; // 完成中序序列in的切割
                pl1 = pl + 1;
                pu1 = pl + (iu1 - il1 + 1);
                pl2 = pu1 + 1;
                pu2 = pu; // 完成先徐序列pre的切割
            }
        }
        t->lchild = getTree(fir, pl1, pu1, mid, il1, iu1);
        t->rchild = getTree(fir, pl2, pu2, mid, il2, iu2);
        return t;
    }

    // 读取压缩文件，解压
    static void unzipFile(char zfile[], char ofile[])
    {
        FILE *fp = fopen(zfile, "rb");
        if (fp == nullptr)
        {
            printf("无法打开压缩文件进行读取!\n");
            exit(0);
        }
        long treesize1, zipsize1, fsize1;            // 树的根结点下标
        fread(&treesize1, sizeof(treesize1), 1, fp); // 哈夫曼树的大小(字节数）
        fread(&zipsize1, sizeof(zipsize1), 1, fp);   // 压缩后内容的大小
        fread(&fsize1, sizeof(fsize1), 1, fp);       // 被压缩内容的大小

        auto *zcontent = static_cast<unsigned char *>(malloc(zipsize1)); // 存放读取的编码数据
        auto *ocontent = static_cast<unsigned char *>(malloc(fsize1));   // 解码后的数据
        const int m = treesize1 / sizeof(HTNode);
        HTNode midArray[m];
        HTNode firArray[m];
        fread(firArray, treesize1, 1, fp); // 读取先序哈夫曼树
        fread(midArray, treesize1, 1, fp); // 读取中序哈夫曼树
        printf("先序哈夫曼树：");
        for (int i = 0; i < m; i++)
            printf("%d ", firArray[i].id);
        printf("\n");
        printf("中序哈夫曼树：");
        for (int i = 0; i < m; i++)
            printf("%d ", midArray[i].id);
        printf("\n");
        fread(zcontent, zipsize1, 1, fp); // 读取编码数据
        fclose(fp);

        HTNode *root = getTree(firArray, 0, m - 1, midArray, 0, m - 1);

        const HTNode *node = root;
        printTree(root);
        fp = fopen(ofile, "wb");
        if (fp == nullptr)
        {
            printf("无法打开解压后文件进行解压!\n");
            exit(0);
        } // 从根开始
        int k = 0, i = 0; // zcontent的下标 k,ocontent的下标i
        while (i < fsize1)
        {
            // 依次写入解压后数据的每个字节
            int j = 128; // 10000000
            while (j > 0)
            {
                if (node)
                {
                    if ((zcontent[k] & j) > 0)
                    {
                        // 向右走 1
                        if (!node->rchild)
                        {
                            ocontent[i++] = node->id; // 解码了下一个字符的第一个bit
                            fprintf(fp, "%c", ocontent[i - 1]);
                            if (i >= fsize1) // 解码结束
                                break;
                            node = root;
                            j = j << 1;
                        }
                        else
                            node = node->rchild;
                    }
                    else
                    {
                        // 向左走 0
                        if (!node->lchild)
                        {
                            ocontent[i++] = node->id; // 解码了下一个字符的第一个bit
                            fprintf(fp, "%c", ocontent[i - 1]);
                            if (i >= fsize1) // 解码结束
                                break;
                            node = root;
                            j = j << 1;
                        }
                        else
                            node = node->lchild;
                    }
                }
                j = j >> 1; // j控制while循环8次，求出zcontent[k]的每一位
            }
            if (i >= fsize1) // 解码结束
                break;
            k++; // 准备读取下一个字符
        }
        fclose(fp);
        free(ocontent);
        free(zcontent);
        printf("文件 %s 已经成功解压为 %s ！\n", zfile, ofile);
    }

    void zipFile(char infile[], char fout[])
    {
        long *w = new long[LEAF];
        long fsize;
        unsigned char *content = parseFile(infile, w, &fsize);
        _hftree = getHFTree(w);
        _hfcode = new HCode[LEAF];
        genHCodes(w);
        // 编码后的内容
        long zipsize; // 压缩后文件大小
        auto *zipContent = static_cast<unsigned char *>(malloc(sizeof(unsigned char) * (fsize + 10000)));
        // 压缩后的文件可能更大，考虑将fsize扩大一点
        if (!zipContent)
        {
            printf("文件太大，内存不够，读入错误!\n");
            exit(0);
        }
        encode(content, fsize, zipContent, &zipsize); // 编码后返回长度zipsize的内容zipContent

        FILE *fp = fopen(fout, "wb");
        if (fp == nullptr)
        {
            printf("无法打开写入文件!\n");
            exit(0);
        }
        int m = getsize(_hftree);
        long treesize = getsize(_hftree) * sizeof(HTNode);
        HTNode midArray[m];
        HTNode firArray[m];
        int i = 0;
        mid_hftree(_hftree, i, midArray);
        i = 0;
        fst_hftree(_hftree, i, firArray);
        printTree(_hftree);

        fwrite(&treesize, sizeof(treesize), 1, fp);             // 保存哈夫曼树的大小
        fwrite(&zipsize, sizeof(zipsize), 1, fp);               // 保存编码内容的大小
        fwrite(&fsize, sizeof(fsize), 1, fp);                   // 保存原始内容的大小
        fwrite(firArray, sizeof(HTNode), m, fp);                // 保存先序哈夫曼树
        fwrite(midArray, sizeof(HTNode), m, fp);                // 保存中序哈夫曼树
        fwrite(zipContent, sizeof(unsigned char), zipsize, fp); // 保存编码后的内容
        fclose(fp);

        free(zipContent); // 释放文件内容
        printf("压缩文件 %s 已经生成！\n", fout);
    }
};

int main()
{
    HFzip hf;
    char infile1[] = "example/11.drawio";       // 源文件
    char zipfile1[256] = "zip/11.drawio.lzip";  // 压缩后的文件
    char unzipfile1[256] = "unzip/111.drawiop"; // 解压后的文件
    hf.zipFile(infile1, zipfile1);
    hf.unzipFile(zipfile1, unzipfile1);
}
#endif
