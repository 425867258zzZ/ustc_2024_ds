#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#define MAXLEN 100

class SString
{
private:
    int len;
    char *base;

public:
    SString() : len(0)
    {
        base = (char *)malloc(sizeof(char));
        base[0] = '\0';
    };

    SString(char *ch)
    {
        base = (char *)malloc(sizeof(ch) * sizeof(char));
        len = 0;
        int i = 0;
        while (ch[i] != '\0')
        {

            base[i] = ch[i];
            len++;
            i++;
        }

        base[len] = '\0';
    }

    // 可通过[]取元素引用,不可修改
    const char &operator[](int i) const
    {
        std::ostringstream oss;
        if (i < 0 || i > len - 1)
        {
            oss << "i: " << i << "(operator[])index out of range我草你妈了个逼傻逼cpp";
            std::string msg = oss.str();
            throw msg;
        }
        return base[i];
    }

    // 返回串长度
    int length() const
    {
        return this->len;
    }

    // 置空串
    void SetEmpty()
    {
        len = 0;
        base[0] = '\0';
    }

    // 返回子串
    SString SubStr(int start, int length = -1)
    {
        length = length == -1 ? len - start : length;
        if (start < 0 || start + length > len)
            throw "index out of range";
        char ch[MAXLEN];
        int i = 0;
        for (i = start; i < start + length; i++)
        {
            ch[i - start] = base[i];
        }
        ch[i - start] = '\0';
        return SString(ch);
    }

    // 比较两个串，相等返回trye
    bool equal(SString str)
    {
        if (len != str.length())
            return false;
        for (int i = 0; i < len; i++)
        {
            if (base[i] != str[i])
                return false;
        }
        return true;
    }

    int *Next(SString str)
    {
        int strlen = str.length();
        int *next = new int[strlen];
        next[0] = -1;
        next[1] = 0;
        for (int j = 2; j < strlen; j++)
        {

            // 计算next数组值
            int flag = 0;
            for (int k = j - 2; k > 0; k--)
            {
                if ((*this)[k] == str[j - 1])
                {
                    if (SubStr(0, k + 1).equal(SubStr(j - k - 1, k + 1)))
                    {
                        next[j] = k + 1;
                        flag = 1;
                    }
                }
                if (!flag)
                    next[j] = 0;
            }

            // 计算next修正值
            for (int j = 1; j < strlen; j++)
            {
                int k = next[j];
                if (str[k] == str[j])
                    next[j] = next[k];
            }
        }
        return next;
    }
    int LocateStr(SString str, int *next = nullptr)
    {
        int strlen = str.length();
        if (!next)
        {
            next = Next(str);
        }
        int n = 0, m = 0;
        while (n < len && m < str.length())
        {
            if (m == -1 || (*this)[n] == str[m])
            {
                n++;
                m++;
            }
            else
                m = next[m];
        }
        return m >= strlen ? n - strlen : -1;
    }

    // 用 V 替换主串 S 中出现的所有与 T 相等的不重叠的子串
    void Replace(SString T, SString V)
    {
        int *next = Next(T);

        int flag = 0;
        int index = LocateStr(T, next);
        if (index != -1)
            flag = 1;
        while (flag)
        {
            len += V.length() - T.length();
            base = (char *)realloc(base, (len + 1) * sizeof(char));
            for (int i = len - 1; i >= index + T.length(); i--)
            {
                base[i] = base[i - (V.length() - T.length())];
            }
            for (int j = index; j < index + V.length(); j++)
            {
                base[j] = V[j - index];
            }
            if (index + V.length() + 1 >= len)
                break;
            SString sub = SubStr(index + V.length());

            int nextindex = sub.LocateStr(T, next);
            if (nextindex == -1)
                flag = 0;
            else
                index = sub.LocateStr(T, next) + index + V.length();
        }
    }

    // 字符串拼接
    SString operator+(SString str)
    {
        if (len + str.length() > MAXLEN)
            throw "(operator+)length out of range";
        char *newbase = new char[MAXLEN + 1];
        int i = 0;
        for (i = 0; i < len; i++)
        {
            newbase[i] = base[i];
        }

        for (i = i; i < len + str.length(); i++)
        {
            newbase[i] = str[i - len];
        }
        newbase[i] = '\0';
        return SString(newbase);
    }

    friend std::ostream &operator<<(std::ostream &os, const SString &str)
    {
        for (int i = 0; i < str.length(); i++)
        {
            os << str[i];
        }
        return os;
    }
};

int main()
{
    try
    {
        char ch1[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
        char ch2[6] = {'w', 'o', 'r', 'l', 'd', '\0'};
        SString str1(ch1);
        SString str2(ch2);
        std::cout << "str1: " << str1 << std::endl;
        std::cout << "str2: " << str2 << std::endl;
        std::cout << "str1.substr(1,3): " << str1.SubStr(1, 3) << std::endl;
        std::cout << "lenstr1: " << str1.length() << std::endl;
        std::cout << "str1+str2: " << str1 + str2 << std::endl;

        char sub[6] = {'l', 'l', '\0'};
        SString substr(sub);
        std::cout << "sub: " << sub << std::endl;
        std::cout << "kmpindex(str1,sub): " << str1.LocateStr(substr) << std::endl;

        char S1[9] = {'l', 'a', 'l', 'l', 'a', 'l', 'l', 'l', '\0'};
        SString S(S1);

        char V1[4] = {'l', 'l', 'b', '\0'};
        SString V(V1);

        char T1[3] = {'l', 'a', '\0'};
        SString T(T1);

        std::cout << "S: " << S << std::endl;
        std::cout << "V: " << V << std::endl;
        std::cout << "T: " << T << std::endl;
        S.Replace(T, V);
        std::cout << "Replace(S,V,T): " << S << std::endl;
    }
    catch (const std::string &msg)
    {
        std::cout << msg << "";
    }
}
