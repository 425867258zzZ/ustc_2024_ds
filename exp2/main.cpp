#include "high_pcs_num.cpp"
#include "high_pcs_rnum.h"
#define N 200

int main()
{

    HighPcsNum hpn1 = HighPcsNum("204.2", 10);
    HighPcsNum hpn2 = HighPcsNum("-11001100.00110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011001100110011", 2);
    cout << "test1: (1) " << HighPcsNum::Base10ToBaseN(hpn1, 2, N).value() << endl;
    cout << "tese1: (2) " << HighPcsNum::Base2ToBase10(hpn2).value() << endl;

    HighPcsNum hpn3 = HighPcsNum("114.5141919810", 10);
    HighPcsNum hpn4 = HighPcsNum("-114.5141919810", 10);
    cout << "test2: (1) " << HighPcsNum::Base10ToBaseN(hpn3, 5, N).value() << endl;
    cout << "test2: (2) " << HighPcsNum::Base10ToBaseN(hpn4, 19, N).value() << endl;

    HighPcsNum hpn5 = HighPcsNum("1.14514", 10);
    HighPcsNum hpn6 = HighPcsNum("-1.919810", 10);
    string tmp1 = (hpn5 + hpn6).value();
    string tmp2 = (hpn5 - hpn6).value();
    string tmp3 = (hpn5 * hpn6).value();
    cout << "test2: (3) 十进制a+b: " << tmp1 << endl;
    cout << "test2: (3) 十进制a-b: " << tmp2 << endl;
    cout << "test2: (3) 十进制a*b: " << tmp3 << endl;
    cout << "test2: (3) N进制a+b: " << HighPcsNum::Base10ToBaseN(HighPcsNum(tmp1, 10), 7, N).value() << endl;
    cout << "test2: (3) N进制a-b: " << HighPcsNum::Base10ToBaseN(HighPcsNum(tmp2, 10), 7, N).value() << endl;
    cout << "test2: (3) N进制a*b: " << HighPcsNum::Base10ToBaseN(HighPcsNum(tmp3, 10), 7, N).value() << endl;

    string poly = "12.31379131*x^2+1*x^1+8.2137619836821388*x^0";
    HighPcsNum x("1.612368921371923122414", 10);
    cout << polyValue(poly, x).value() << endl;
    return 0;
}