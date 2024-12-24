#include <iostream>
using namespace std;
inline string remove_dot(string s)
{
    int i = s.find_first_of(".");
    if (i == string::npos) return s;
    else return s.substr(0, i) + s.substr(i + 1, s.size() - i - 1);
}
inline string add(string a, string b, bool order)//两个无小数点的数字相加，不考虑进位,顺序true表示从左到右遍历,false表示从右到左遍历
{
    if (order)
    {
        if (a.size() < b.size()) return add(b, a, true);//保证a的长度>=b的长度
        string result = a;
        for (int i = 0; i < b.size(); i++)
        {
            result[i] += b[i] - '0';
        }
        return result;
    }
    else
    {
        if (a.size() < b.size()) return add(b, a, false);//保证a的长度>=b的长度
        string result = a;
        for (int i = 0; i < b.size(); i++)
        {
            result[a.size() - 1 - i] += b[b.size() - 1 - i] - '0';
        }
        return result;
    }
}
inline string oppositestr(string s)
{
    for (int i = 0; i < s.size();i++)
    {
        s[i] = 2 * '0' - s[i];
    }
    return s;
}
inline string carry_out(string s)
{
    for (int i = s.size() - 1; i >= 0; i--)
    {
        if (s[i] > '9')
        {
            if (i == 0)
            {
                s = "0" + s;
                i = 1;
            }
            s[i - 1] += (s[i] - '0') / 10;
            s[i] = (s[i] - '0') % 10 + '0';//注意ascll码转成数字再求余数
        }
        if (s[i] < '0')
        {
            s[i - 1] -= ('0' - s[i]) / 10 + 1;
            s[i] += (('0' - s[i]) / 10 + 1) * 10;//不要轻易删掉/10 *10，你别以为他没用，其实他其中蕴含了取整的操作
        }
    }
    return s;
}
enum numsign { positive = 1, zero = 0, negative = -1 };
class bignum
{
public:
    string abs_value;//这个是去除符号和小数点后的一串数字
    string real_value;//这个是有符号和小数点，并且要显示给用户看的数字
    numsign sign = zero;//符号
    int poipos = 0;//小数点位置，这里我想表示的是小数位数，即从右边第几位开始是小数点
    bignum()
    {
        sign = zero;
        abs_value = "0";
        real_value = "0";
        poipos = 0;
    }
    bignum(numsign sign, string abs_value, int poipos)
    {
        this->sign = sign;
        this->abs_value = abs_value;
        this->poipos = poipos;
        if (poipos != 0)
            this->real_value = (sign == negative ? "-" : "") + abs_value.substr(0, abs_value.size() - poipos) + "." + abs_value.substr(abs_value.size() - poipos, poipos);
        else
            this->real_value = (sign == negative ? "-" : "") + abs_value;
    }
    bignum(string s)
    {
        if (s == "0")//0
        {
            sign = zero;
            abs_value = "0";
            real_value = "0";
            poipos = 0;
            return;
        }
        switch (s[0])//判断符号
        {
        case '-':
            sign = negative;
            break;
        default:
            s = '+' + s;
        case '+':
            sign = positive;
            break;
        }
        poipos = s.size() - 1;//从最后一位开始
        while (s[poipos] != '.' && poipos > 0)
        {
            poipos--;
        }
        if (poipos == 0)//说明找不到小数点
            s += ".";

        poipos = s.size() - 1 - poipos;//计算小数部分位数
        int start = 1;
        for (; start < s.size();start++)//找到有效数字的起始位置
        {
            if (s[start] != '0')
            {
                if (s[start] == '.')//说明第一个非零字符为小数点，如+0.01
                {
                    start--;
                }
                break;
            }
        }
        int end = s.size() - 1;
        for (; end > 0;end--)
        {
            if (s[end] != '0')
            {
                if (s[end] == '.')//说明最后一个非零字符为小数点，如1.0
                {
                    end--;
                    poipos = 0;
                }
                else if (s.size() - end - 1 < poipos)
                {
                    poipos -= s.size() - end - 1;
                }
                break;
            }
        }
        abs_value = remove_dot(s.substr(start, end - start + 1));
        if (poipos != 0)
            real_value = (sign == negative ? "-" : "") + abs_value.substr(0, abs_value.size() - poipos) + "." + abs_value.substr(abs_value.size() - poipos, poipos);
        else
            real_value = (sign == negative ? "-" : "") + abs_value;
    }
    bignum opposite() const//符号位取反
    {
        return bignum((numsign)-sign, abs_value, poipos);
    }
    bignum abs() const//取绝对值
    {
        return bignum(positive, abs_value, poipos);
    }
    int compare(const bignum& b) const//>0:a>b,<0:a<b,=0:a==b
    {
        if (sign != b.sign)
            return sign - b.sign;
        if (sign == negative && b.sign == negative)
        {
            return b.opposite().compare((*this).opposite());
        }
        if (abs_value.size() - poipos > b.abs_value.size() - b.poipos)//如果a的整数位数多于b，则a>b
        {
            return 1;
        }
        else if (abs_value.size() - poipos < b.abs_value.size() - b.poipos)
        {
            return -1;
        }
        else//整数位数相同
        {
            if (abs_value.size() < b.abs_value.size())//确保a的全部位数>=b的全部位数（整数部分相同的前提下，也就是a的小数部分一定不短于b的小数部分）
            {
                return -(b.compare(*this));
            }
            int i = 0;
            for (; i < b.abs_value.size(); i++)
            {
                if (abs_value[i] != b.abs_value[i])
                {
                    return abs_value[i] - b.abs_value[i];
                }
            }
            //到这里，说明一直到他们共有的位数，a的小数部分和b的小数部分都相同
            if (i == abs_value.size())//说明a和b相同
            {
                return 0;
            }
            else//说明a的小数部分比b的小数部分长
            {
                return 1;
            }
        }
    }
    bignum& operator=(const bignum& b)
    {
        sign = b.sign;
        abs_value = b.abs_value;
        real_value = b.real_value;
        poipos = b.poipos;
        return *this;
    }
    bignum operator+(const bignum& b) const
    {
        if (sign == b.sign)//同号逻辑
        {
            if (poipos < b.poipos)//确保a的小数部分位数>=b的小数部分位数
            {
                return b + *this;
            }
            string result_xiao = "";
            string xiaob = "";
            if (poipos) result_xiao = abs_value.substr(abs_value.size() - poipos, poipos);//a的小数部分
            if (b.poipos)
            {
                xiaob = b.abs_value.substr(b.abs_value.size() - b.poipos, b.poipos);//b的小数部分
                for (int i = 0; i < xiaob.size(); i++)
                {
                    result_xiao[i] += xiaob[i] - '0';
                }
            }
            //下面开始计算整数部分
            string zhenga = abs_value.substr(0, abs_value.size() - poipos);
            string zhengb = b.abs_value.substr(0, b.abs_value.size() - b.poipos);
            string result_zheng = add(zhenga, zhengb, false);
            string all = result_zheng + result_xiao;
            string all_with_carry = carry_out(all);
            if (poipos)
            {
                return bignum((sign == negative ? "-" : "") + all_with_carry.substr(0, all_with_carry.size() - poipos) + "." + all_with_carry.substr(all_with_carry.size() - poipos, poipos));
            }
            else
            {
                return bignum((sign == negative ? "-" : "") + all_with_carry);
            }
        }
        else if (sign == zero)
        {
            return b;
        }
        else if (b.sign == zero)
        {
            return *this;
        }
        else//异号逻辑
        {
            if (sign == negative) return b + *this;//确保a是正数，b是负数
            numsign sign_result = positive;
            if (abs().compare(b.abs()) > 0)//|a| > |b|
            {
                sign_result = sign;
            }
            else if (abs().compare(b.abs()) < 0)
            {
                sign_result = b.sign;
            }
            else//|a| == |b|且符号不同,因此a=-b,a+b=0
            {
                return bignum();
            }
            if (sign_result == negative)//确保结果为正,把3+(-6)转化为-(  6+(-3)  ),那如果是-6+3呢？-6+3=3+(-6)（最开始的a是正数，b是负数）=-(  6+(-3)  )
            {
                return (b.opposite() + opposite()).opposite();
            }
            bignum big = abs() + bignum(positive, oppositestr(b.abs_value), b.poipos);
            return bignum(sign_result, big.abs_value, big.poipos);
        }
    }
    bignum operator-(const bignum& b) const
    {
        return *this + b.opposite();
    }
    friend ostream& operator<<(ostream& os, const bignum& b)
    {
        os << b.real_value;
        return os;
    }
    friend istream& operator>>(istream& is, bignum& b)
    {
        string s;
        is >> s;
        b = bignum(s);
        return is;
    }
};
int main()
{
    bignum a, b;
    cin >> a >> b;
    cout << (a + b);
    return 0;
}