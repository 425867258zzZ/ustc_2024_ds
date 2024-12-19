#include "high_pcs_rnum.h"
#include <cmath>
#include <queue>

HighPcsNum::HighPcsNum()
{
    is_positive = true;
    base = 10;
}

HighPcsNum::HighPcsNum(string num_str, int base)
{
    this->base = base;
    int len = static_cast<int>(num_str.length());
    int dot_index = num_str.find('.') == std::string::npos ? -1 : static_cast<int>(num_str.find('.'));
    is_positive = num_str[0] != '-';

    if (dot_index == -1)
    {
        int i = is_positive ? 0 : 1;
        for (; i < len; i++)
        {
            interger.push_back(static_cast<int>(num_str[i]) - '0');
        }
    }
    else
    {
        int j = is_positive ? 0 : 1;
        for (; j < len; j++)
        {
            if (j < dot_index)
                interger.push_back(static_cast<int>(num_str[j]) - '0');
            if (j > dot_index)
                fraction.push_back(static_cast<int>(num_str[j]) - '0');
        }
    }
    RemoveTrailingZeros(fraction);
};

HighPcsNum::HighPcsNum(const int base, const bool is_positive, list<int> integer, list<int> fraction)
{
    this->base = base;
    this->is_positive = is_positive;
    this->interger = integer;
    this->fraction = fraction;
}

// 返回值格式化字符串
string HighPcsNum::value() const
{
    string result;
    const string letters = "ABCDEFGHIJ";
    if (!is_positive)
    {
        result += "-";
    }
    if (interger.empty() && !fraction.empty())
    {
        result += "0";
    }
    for (int i : interger)
    {
        if (i < 10)
            result += to_string(i);
        else
            result += letters[i - 10];
    }
    if (!fraction.empty())
    {
        result += ".";
        for (int i : fraction)
        {
            if (i < 10)
                result += to_string(i);
            else
                result += letters[i - 10];
        }
    }
    return result;
}

// 小数比较，返回-1,0,1 代表小于，等于，大于
int HighPcsNum::compareFra(list<int> list1, list<int> list2)
{
    int len1 = list1.size();
    int len2 = list2.size();

    auto it1 = list1.begin();
    auto it2 = list2.begin();
    for (int i = 0; i < min(len1, len2); ++i, ++it1, ++it2)
    {
        if (*it1 != *it2)
            return (*it1 > *it2) ? 1 : -1;
    }
    if (len1 > len2)
        return 1;
    else if (len1 == len2)
        return 0;
    else
        return -1;
}

// 整数比较，返回-1,0,1 代表小于，等于，大于
int HighPcsNum::compareInt(list<int> list1, list<int> list2)
{
    int len1 = list1.size();
    int len2 = list2.size();
    if (len1 != len2)
    {
        if (len1 > len2)
            return 1;
        if (len1 < len2)
            return -1;
    }
    auto it1 = list1.begin();
    auto it2 = list2.begin();
    for (int i = 0; i < min(len1, len2); ++i, ++it1, ++it2)
    {
        if (*it1 != *it2)
            return (*it1 > *it2) ? 1 : -1;
    }
    return 0;
}

// 1减去某个小数
list<int> HighPcsNum::fracComplement(list<int> frac, int base)
{
    list<int> res;
    auto it = frac.rbegin();
    auto end = frac.rend();
    int borrow = 0;
    while (it != end)
    {
        res.push_front(borrow - *it + base);
        borrow = -1;
        ++it;
    }
    return res;
}

// 移除小数部分的尾导零
void HighPcsNum::RemoveTrailingZeros(std::list<int> &numList)
{
    auto it = std::find_if(numList.rbegin(), numList.rend(), [](int digit)
                           { return digit != 0; });
    if (it != numList.rend())
    {
        numList.erase(++std::next(it).base(), numList.end());
    }
    else
    {
        numList.clear();
    }
}

// 整数部分加法，保证两个数皆为正,进制相同
list<int> HighPcsNum::intAdd(list<int> integer1, list<int> integer2, int base, int carry)
{
    list<int> res_integer;
    if (integer1.empty())
    {
        res_integer.assign(integer2.begin(), integer2.end());
    }
    else if (integer2.empty())
    {
        res_integer.assign(integer1.begin(), integer1.end());
    }
    else
    {
        auto n1 = integer1.rbegin();
        auto end1 = integer1.rend();
        auto n2 = integer2.rbegin();
        auto end2 = integer2.rend();
        while (n1 != end1 || n2 != end2)
        {
            int num1 = n1 == end1 ? 0 : *n1;
            int num2 = n2 == end2 ? 0 : *n2;

            int sum = (num1 + num2 + carry) % base;
            carry = (num1 + num2 + carry) / base;

            res_integer.push_front(sum);
            if (n1 != end1)
                ++n1;
            if (n2 != end2)
                ++n2;
        }
    }
    if (carry > 0)
        res_integer.push_front(1);
    return res_integer;
}

// 整数部分减法，保证两个数皆为正,进制相同，borrow记录正负号
list<int> HighPcsNum::intSub(list<int> integer1, list<int> integer2, int base, int &flag)
{
    list<int> res_integer;
    if (integer1.empty())
    {
        res_integer.assign(integer2.begin(), integer2.end());
        flag = -1;
    }
    else if (integer2.empty())
    {
        res_integer.assign(integer1.begin(), integer1.end());
        flag = 0;
    }
    else
    {
        list<int> i1;
        list<int> i2;

        if (compareInt(integer1, integer2) > 0)
        {
            i1.assign(integer1.begin(), integer1.end());
            i2.assign(integer2.begin(), integer2.end());
            flag = 0;
        }
        else if (compareFra(integer1, integer2) == 0)
        {
            flag = 0;
            return res_integer;
        }
        else
        {
            i1.assign(integer2.begin(), integer2.end());
            i2.assign(integer1.begin(), integer1.end());
            flag = -1;
        }
        int borrow = 0;
        auto n1 = i1.rbegin();
        auto end1 = i1.rend();
        auto n2 = i2.rbegin();
        auto end2 = i2.rend();
        while (n1 != end1 || n2 != end2)
        {
            int num1 = n1 == end1 ? 0 : *n1;
            int num2 = n2 == end2 ? 0 : *n2;
            int sub = (num1 + borrow - num2) % base;
            sub = sub < 0 ? sub + base : sub;
            borrow = num1 + borrow - num2 < 0 ? -1 : 0;
            res_integer.push_front(sub);
            if (n1 != end1)
                ++n1;
            if (n2 != end2)
                ++n2;
        }
    }
    return res_integer;
}

// 小数部分加法，保证两个数皆为正,进制相同
list<int> HighPcsNum::fracAdd(list<int> fraction1, list<int> fraction2, int base, int &carry)
{
    list<int> res_fraction;
    if (fraction1.empty())
    {
        res_fraction.assign(fraction2.begin(), fraction2.end());
    }
    else if (fraction2.empty())
    {
        res_fraction.assign(fraction1.begin(), fraction1.end());
    }
    else
    {
        auto n1 = fraction1.rbegin();
        auto end1 = fraction1.rend();
        auto n2 = fraction2.rbegin();
        auto end2 = fraction2.rend();

        int delta = fraction1.size() - fraction2.size();
        int padding = abs(delta);
        auto *pad_n = delta > 0 ? &n1 : &n2;
        for (; padding > 0; padding--)
        {
            res_fraction.push_front(**pad_n);
            ++(*pad_n);
        }

        while (n1 != end1 && n2 != end2)
        {
            int sum = (*n1 + *n2 + carry) % base;
            carry = (*n1 + *n2 + carry) / base;

            res_fraction.push_front(sum);
            ++n1;
            ++n2;
        }
    }
    RemoveTrailingZeros(res_fraction);
    return res_fraction;
}

// 小数部分减法，保证两个数皆为正,进制相同，flag记录正负号
list<int> HighPcsNum::fracSub(list<int> fraction1, list<int> fraction2, int base, int &flag)
{
    list<int> res_fraction;
    if (fraction1.empty())
    {
        res_fraction.assign(fraction2.begin(), fraction2.end());
    }
    else if (fraction2.empty())
    {
        res_fraction.assign(fraction1.begin(), fraction1.end());
        flag = -1;
    }
    else
    {
        list<int> f1;
        list<int> f2;

        if (compareFra(fraction1, fraction2) > 0)
        {
            f1.assign(fraction1.begin(), fraction1.end());
            f2.assign(fraction2.begin(), fraction2.end());
            flag = 0;
        }
        else if (compareFra(fraction1, fraction2) == 0)
        {
            flag = 0;
            return res_fraction;
        }
        else
        {
            f1.assign(fraction2.begin(), fraction2.end());
            f2.assign(fraction1.begin(), fraction1.end());
            flag = -1;
        }
        auto n1 = f1.rbegin();
        auto end1 = f1.rend();
        auto n2 = f2.rbegin();
        auto end2 = f2.rend();

        int delta = f1.size() - f2.size();
        int padding = abs(delta);
        int flag1 = delta > 0 ? 1 : 0;
        int borrow = 0;
        while (n1 != end1 && n2 != end2)
        {
            int num1, num2;
            if (padding > 0)
            {
                num1 = flag1 ? *n1 : 0;
                num2 = !flag1 ? *n2 : 0;
            }
            else
            {
                num1 = *n1;
                num2 = *n2;
            }
            int sub = (num1 + borrow - num2) % base;
            sub = sub < 0 ? (sub + base) % 10 : sub;
            borrow = num1 + borrow - num2 < 0 ? -1 : 0;
            res_fraction.push_front(sub);
            if (padding > 0)
            {
                n1 = flag1 ? ++n1 : n1;
                n2 = !flag1 ? ++n2 : n2;
            }
            else
            {
                ++n1;
                ++n2;
            }
            padding--;
        }
    }
    RemoveTrailingZeros(res_fraction);
    return res_fraction;
}

// 高精度数加法，进制相同
HighPcsNum HighPcsNum::add(const HighPcsNum &hpn1, const HighPcsNum &hpn2) const
{
    if ((hpn1.is_positive && hpn2.is_positive) || (!hpn1.is_positive && !hpn2.is_positive))
    {
        int tmp = 0;
        list<int> res_frac = fracAdd(hpn1.fraction, hpn2.fraction, hpn1.base, tmp);
        list<int> res_int = intAdd(hpn1.interger, hpn2.interger, hpn1.base, tmp);

        return HighPcsNum(base, hpn1.is_positive, res_int, res_frac);
    }
    else
    {
        int tmp1 = 0;
        list<int> res_frac = fracSub(hpn1.fraction, hpn2.fraction, hpn1.base, tmp1);

        int tmp2 = 0;
        list<int> res_int = intSub(hpn1.interger, hpn2.interger, hpn1.base, tmp2);

        if ((tmp1 == 0 && tmp2 == 0) || (tmp1 != 0 && tmp2 != 0))
        {
            return HighPcsNum(base, tmp2 == 0 ? false : true, res_int, res_frac);
        }
        else if (tmp1 != 0 && tmp2 == 0)
        {
            int _ = 0;
            list<int> res_int1 = !res_int.empty() ? intSub(res_int, list<int>{1}, hpn1.base, _) : res_int;
            list<int> res_frac1 = !res_int.empty() ? fracComplement(res_frac, hpn1.base) : res_frac;
            return HighPcsNum(base, tmp1 == 0 ? true : false, res_int1, res_frac1);
        }
        else
        {
            int _ = 0;
            list<int> res_int1 = !res_frac.empty() ? intSub(res_int, list<int>{1}, hpn1.base, _) : res_int;
            list<int> res_frac1 = fracComplement(res_frac, hpn1.base);
            return HighPcsNum(base, true, res_int1, res_frac1);
        }
    }
}

HighPcsNum HighPcsNum::mul(const HighPcsNum &hpn1, const HighPcsNum &hpn2) const
{
    list<int> l1;
    l1.insert(l1.begin(), hpn1.interger.begin(), hpn1.interger.end());
    l1.insert(l1.end(), hpn1.fraction.begin(), hpn1.fraction.end());

    list<int> l2;
    l2.insert(l2.begin(), hpn2.interger.begin(), hpn2.interger.end());
    l2.insert(l2.end(), hpn2.fraction.begin(), hpn2.fraction.end());

    int dot_count = hpn1.fraction.size() + hpn2.fraction.size();
    int zero_count = 0;

    list<list<int>> tmps;
    for (auto i = l1.rbegin(); i != l1.rend(); ++i, zero_count++)
    {
        int carry = 0;
        list<int> tmp;
        for (auto j = l2.rbegin(); j != l2.rend(); ++j)
        {
            int mul = ((*i) * (*j) + carry) % base;
            carry = ((*i) * (*j) + carry) / base;
            tmp.push_front(mul);
        }
        if (carry > 0)
        {
            tmp.push_front(carry);
        }
        for (int k = 0; k < zero_count; k++)
            tmp.push_back(0);
        tmps.push_front(tmp);
    }
    list<int> whole_num;
    list<int> res_integer;
    list<int> res_fraction;

    for (list<int> tmp : tmps)
    {
        int tmp1 = 0;
        whole_num = intAdd(whole_num, tmp, hpn1.base, tmp1);
    }
    auto it = whole_num.rbegin();
    for (int i = 0; i < dot_count; i++, ++it)
    {
        res_fraction.push_front(*it);
    }
    RemoveTrailingZeros(res_fraction);
    for (; it != whole_num.rend(); ++it)
    {
        res_integer.push_front(*it);
    }
    return HighPcsNum(base, hpn1.is_positive == hpn2.is_positive, res_integer, res_fraction);
}

HighPcsNum HighPcsNum::operator+(const HighPcsNum &hpn) const
{
    if (!(this->is_positive && !hpn.is_positive))
        return add(*this, hpn);
    else
        return add(hpn, *this);
}

HighPcsNum HighPcsNum::operator-(const HighPcsNum &hpn) const
{
    if (this->is_positive && hpn.is_positive)
        return add(HighPcsNum(base, false, hpn.interger, hpn.fraction), *this);
    else if (this->is_positive && !hpn.is_positive)
        return add(*this, HighPcsNum(base, true, hpn.interger, hpn.fraction));
    else
        return add(HighPcsNum(base, false, this->interger, this->fraction),
                   HighPcsNum(base, false, hpn.interger, hpn.fraction));
}

HighPcsNum HighPcsNum::operator*(const HighPcsNum &hpn) const
{
    return mul(*this, hpn);
}

HighPcsNum HighPcsNum::Base10ToBaseN(HighPcsNum hpn, int n, int N)
{
    if (hpn.base != 10)
    {
        cout << "ERROR:输入数不是十进制数！" << endl;
        return HighPcsNum();
    }
    list<int> res_integer;
    int int_num = 0;
    for (int i : hpn.interger)
        int_num = int_num * 10 + i;
    while (int_num > 0)
    {
        res_integer.push_front(int_num % n);
        int_num /= n;
    }

    list<int> res_fraction;
    list<int> tmp_frac;
    tmp_frac.assign(hpn.fraction.begin(), hpn.fraction.end());

    for (int i = 0; i < N; i++)
    {
        list<int> frac_left;
        int frac_num = 0;
        for (int j = 0; j < n; j++)
        {
            int carry = 0;
            frac_left = fracAdd(frac_left, tmp_frac, 10, carry);
            frac_num += carry;
        }
        res_fraction.push_back(frac_num);
        tmp_frac.assign(frac_left.begin(), frac_left.end());
    }

    return HighPcsNum(n, hpn.is_positive, res_integer, res_fraction);
}

HighPcsNum HighPcsNum::Base2ToBase10(HighPcsNum hpn)
{
    if (hpn.base == 10)
        return hpn;
    if (hpn.base != 2)
    {
        cout << "ERROR:输入数不是二进制数！" << endl;
        return HighPcsNum();
    }
    list<int> res_int;
    int int_count = 0;
    for (auto it = hpn.interger.rbegin(); it != hpn.interger.rend(); ++it)
    {
        string num_str = to_string((*it * pow(hpn.base, int_count)));
        res_int = intAdd(res_int, HighPcsNum(num_str, 10).interger, 10, 0);
        int_count++;
    }

    list<int> res_frac;

    int frac_count = 1;
    for (int i : hpn.fraction)
    {
        HighPcsNum tmp_num("0.5", 10);
        HighPcsNum tmp_res("1", 10);
        HighPcsNum res("0", 10);
        for (int j = 0; j < frac_count; j++)
            tmp_res = tmp_res * tmp_num;
        res = tmp_num + tmp_res * HighPcsNum(to_string(i), 10);
        int tmp = 0;
        res_frac = fracAdd(res_frac, res.fraction, 10, tmp);
        frac_count++;
    }

    return HighPcsNum(10, hpn.is_positive, res_int, res_frac);
}

typedef struct pnode
{
    HighPcsNum coef;
    int expn;
} pnode;

queue<pnode> readPoly(string str)
{
    list<int> coef_index;
    if (str[0] != '-')
        str = "+" + str;
    for (int i = 0; i < static_cast<int>(str.size()); i++)
    {
        if (str[i] == '+' || str[i] == '-')
            coef_index.push_back(i); // 记录系数位置
    }

    queue<pnode> poly;
    for (auto it = coef_index.begin(); it != coef_index.end(); ++it)
    {
        int start = *it + 1;
        int end = next(it, 1) == coef_index.end() ? str.size() : *(next(it, 1));
        string part = str.substr(start, end - start);
        if (!part.empty())
        {
            int mul_index = part.find('*') == std::string::npos ? -1 : static_cast<int>(part.find('*'));
            string coef_string = part.substr(0, mul_index);
            if (str[*it] == '-')
                coef_string = "-" + coef_string;
            HighPcsNum coef(coef_string, 10);
            pnode pn;
            string expn = part.substr(mul_index + 3);
            int expn_int = stoi(expn);
            pn = {
                coef,
                expn_int,
            };
            poly.push(pn);
        }
    }
    return poly;
}

HighPcsNum polyValue(string poly_str, HighPcsNum x)
{
    queue<pnode> poly = readPoly(poly_str);
    HighPcsNum res = HighPcsNum("0", 10);
    while (!poly.empty())
    {
        pnode pn = poly.front();
        HighPcsNum coef = pn.coef;
        int expn = pn.expn;
        HighPcsNum pow = HighPcsNum("1", 10);
        for (int i = 0; i < expn; i++)
        {
            pow = pow * x;
        }
        res = res + coef * pow;
        poly.pop();
    }
    return res;
}
