#include <iostream>
using namespace std;

int getleng(string str)//获取字符串长度，不包括前导0
{
	for (int i = 0;i < str.size();i++)
	{
		if (str[i] != '0') return str.size() - i;//我们找的是从左起第一个非0字符的位置
	}
	return 0;
}
string opposite(string a)//相反数，拿来把减法变成加法
{
	string result = a;
	for (int i = 0;i < result.size();i++)
	{
		result[i] = 2 * '0' - result[i];
	}
	return result;
}

string myplus(string a, string b)
{
	int na = getleng(a);
	int nb = getleng(b);
	string reala = "0" + a.substr(a.size() - na, na);//使其前面只有一个0，拿来作进位用
	string realb = "0" + b.substr(b.size() - nb, nb);
	if (reala.size() < realb.size()) return myplus(realb, reala);//确保a的长度大于等于b的长度
	for (int i = 1;i <= realb.size();i++)//这个循环用来把realb的每一位加到reala的对应位上，可能溢出'9'，到达符号区域，如':'
	{
		reala[reala.size() - i] = reala[reala.size() - i] + realb[realb.size() - i] - '0';
	}
	for (int i = reala.size() - 1;i >= 0;i--)//这个循环用来处理进位，从右到左，遇到大于'9'的字符就进位到下一位
	{
		if (reala[i] > '9')
		{
			reala[i - 1] += (reala[i] - '0') / 10;//其实可以直接reala[i-1]++，不过这样写更逆天
			reala[i] = reala[i] - 10 * ((reala[i] - '0') / 10);
		}
		else if (reala[i] < '0')
		{
			reala[i - 1] -= ('0' - reala[i]) / 10 + 1;
			reala[i] = 10 * (('0' - reala[i]) / 10 + 1) + reala[i];
		}
	}
	int nreala = getleng(reala);//去掉前导0
	return reala.substr(reala.size() - nreala, nreala);
}

int mycompare(string a, string b)//比较两个字符串，返回-1,0,1分别表示a<b,a=b,a>b
{
	int na = getleng(a);
	int nb = getleng(b);
	string reala = a.substr(a.size() - na, na);
	string realb = b.substr(b.size() - nb, nb);
	if (reala.size() < realb.size()) return -1;
	else if (reala.size() > realb.size()) return 1;//先按位数来比较
	else//若位数相同，再按每一位来比较
	{
		for (int i = 0;i < reala.size();i++)
		{
			if (reala[i] < realb[i]) return -1;
			else if (reala[i] > realb[i]) return 1;
		}
		return 0;
	}
}
string myminus(string a, string b)//a-b,注意减法是有方向的
{
	int na = getleng(a);
	int nb = getleng(b);
	string reala = a.substr(a.size() - na, na);
	string realb = b.substr(b.size() - nb, nb);
	if (mycompare(reala, realb) > 0) return myplus(reala, opposite(realb));//a>=b时，a-b=a+(-b)
	else if (mycompare(reala,realb) == 0) return "0";//a=b时，a-b=0
	else return "-" + myplus(realb, opposite(reala));//a<b时，a-b=-(b+(-a))
}

int main()
{
	string a, b;
	while (true)
	{
		cout << "input a:";
		cin >> a;
		if (a == "exit") break;
		cout << "input b:";
		cin >> b;
		cout << "a + b =:" << myplus(a, b) << endl;
		cout << "a - b =:" << myminus(a, b) << endl;
	}
	return 0;
}