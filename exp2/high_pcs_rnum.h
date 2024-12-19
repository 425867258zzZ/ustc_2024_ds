#ifndef HighPcsRnum_H_
#define HighPcsRnum_H_

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <queue>

using namespace std;

// 实现n进制高精度数及其ADT
class HighPcsNum
{
public:
    // 分别表示进制，符号，整数部分和小数部分
    int base;
    bool is_positive;
    list<int> interger;
    list<int> fraction;

    HighPcsNum();
    // 输入字符串构造法
    HighPcsNum(string num_str, int base = 10);
    // 按参数构造法
    HighPcsNum(int base, bool is_positive, list<int> integer, list<int> fraction);

    // 返回格式化值串
    string value() const;

    // 比较两个小数大小
    static int compareFra(list<int> list1, list<int> list2);

    // 比较两个整数大小
    static int compareInt(list<int> list1, list<int> list2);

    // 小数部分取补数
    static list<int> fracComplement(list<int> frac, int base);

    // 去除小数部分末尾的0
    static void RemoveTrailingZeros(std::list<int> &numList);

    // 整数部分加法，保证两个数皆为正
    static list<int> intAdd(list<int> integer1, list<int> integer2, int base, int carry);

    // 整数部分减法，保证两个数皆为正，flag记录正负号
    static list<int> intSub(list<int> integer1, list<int> integer2, int base, int &flag);

    // 小数部分加法，保证两个数皆为正，carry记录往整数部分进位情况
    static list<int> fracAdd(list<int> fraction1, list<int> fraction2, int base, int &carry);

    // 小数部分减法，保证两个数皆为正，flag记录正负号
    static list<int> fracSub(list<int> fraction1, list<int> fraction2, int base, int &flag);

    // 高精度数加法
    HighPcsNum add(const HighPcsNum &hpn1, const HighPcsNum &hpn2) const;
    // 高精度数乘法
    HighPcsNum mul(const HighPcsNum &hpn1, const HighPcsNum &hpn2) const;

    // 实现无精度丢失加减乘
    HighPcsNum operator+(const HighPcsNum &hpn) const;
    HighPcsNum operator-(const HighPcsNum &hpn) const;
    HighPcsNum operator*(const HighPcsNum &hpn) const;

    // 10转换为n进制
    static HighPcsNum Base10ToBaseN(HighPcsNum hpn, int n, int N);

    // n转换为10进制
    static HighPcsNum Base2ToBase10(HighPcsNum hpn);
};

struct pnode;
queue<pnode> readPoly(string str);
#endif