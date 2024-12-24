#include <iostream>
using namespace std;
inline string remove_dot(string s)
{
    int i = s.find_first_of(".");
    if (i == string::npos) return s;
    else return s.substr(0, i) + s.substr(i + 1, s.size() - i - 1);
}
inline string add(string a, string b, bool order)//������С�����������ӣ������ǽ�λ,˳��true��ʾ�����ұ���,false��ʾ���ҵ������
{
    if (order)
    {
        if (a.size() < b.size()) return add(b, a, true);//��֤a�ĳ���>=b�ĳ���
        string result = a;
        for (int i = 0; i < b.size(); i++)
        {
            result[i] += b[i] - '0';
        }
        return result;
    }
    else
    {
        if (a.size() < b.size()) return add(b, a, false);//��֤a�ĳ���>=b�ĳ���
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
            s[i] = (s[i] - '0') % 10 + '0';//ע��ascll��ת��������������
        }
        if (s[i] < '0')
        {
            s[i - 1] -= ('0' - s[i]) / 10 + 1;
            s[i] += (('0' - s[i]) / 10 + 1) * 10;//��Ҫ����ɾ��/10 *10�������Ϊ��û�ã���ʵ�������̺���ȡ���Ĳ���
        }
    }
    return s;
}
enum numsign { positive = 1, zero = 0, negative = -1 };
class bignum
{
public:
    string abs_value;//�����ȥ�����ź�С������һ������
    string real_value;//������з��ź�С���㣬����Ҫ��ʾ���û���������
    numsign sign = zero;//����
    int poipos = 0;//С����λ�ã����������ʾ����С��λ���������ұߵڼ�λ��ʼ��С����
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
        switch (s[0])//�жϷ���
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
        poipos = s.size() - 1;//�����һλ��ʼ
        while (s[poipos] != '.' && poipos > 0)
        {
            poipos--;
        }
        if (poipos == 0)//˵���Ҳ���С����
            s += ".";

        poipos = s.size() - 1 - poipos;//����С������λ��
        int start = 1;
        for (; start < s.size();start++)//�ҵ���Ч���ֵ���ʼλ��
        {
            if (s[start] != '0')
            {
                if (s[start] == '.')//˵����һ�������ַ�ΪС���㣬��+0.01
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
                if (s[end] == '.')//˵�����һ�������ַ�ΪС���㣬��1.0
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
    bignum opposite() const//����λȡ��
    {
        return bignum((numsign)-sign, abs_value, poipos);
    }
    bignum abs() const//ȡ����ֵ
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
        if (abs_value.size() - poipos > b.abs_value.size() - b.poipos)//���a������λ������b����a>b
        {
            return 1;
        }
        else if (abs_value.size() - poipos < b.abs_value.size() - b.poipos)
        {
            return -1;
        }
        else//����λ����ͬ
        {
            if (abs_value.size() < b.abs_value.size())//ȷ��a��ȫ��λ��>=b��ȫ��λ��������������ͬ��ǰ���£�Ҳ����a��С������һ��������b��С�����֣�
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
            //�����˵��һֱ�����ǹ��е�λ����a��С�����ֺ�b��С�����ֶ���ͬ
            if (i == abs_value.size())//˵��a��b��ͬ
            {
                return 0;
            }
            else//˵��a��С�����ֱ�b��С�����ֳ�
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
        if (sign == b.sign)//ͬ���߼�
        {
            if (poipos < b.poipos)//ȷ��a��С������λ��>=b��С������λ��
            {
                return b + *this;
            }
            string result_xiao = "";
            string xiaob = "";
            if (poipos) result_xiao = abs_value.substr(abs_value.size() - poipos, poipos);//a��С������
            if (b.poipos)
            {
                xiaob = b.abs_value.substr(b.abs_value.size() - b.poipos, b.poipos);//b��С������
                for (int i = 0; i < xiaob.size(); i++)
                {
                    result_xiao[i] += xiaob[i] - '0';
                }
            }
            //���濪ʼ������������
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
        else//����߼�
        {
            if (sign == negative) return b + *this;//ȷ��a��������b�Ǹ���
            numsign sign_result = positive;
            if (abs().compare(b.abs()) > 0)//|a| > |b|
            {
                sign_result = sign;
            }
            else if (abs().compare(b.abs()) < 0)
            {
                sign_result = b.sign;
            }
            else//|a| == |b|�ҷ��Ų�ͬ,���a=-b,a+b=0
            {
                return bignum();
            }
            if (sign_result == negative)//ȷ�����Ϊ��,��3+(-6)ת��Ϊ-(  6+(-3)  ),�������-6+3�أ�-6+3=3+(-6)���ʼ��a��������b�Ǹ�����=-(  6+(-3)  )
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