
#include <iostream>

using namespace std;
int main()
{
	cout << "欢迎使用物质的量计算器，以下操作中：\n1. 告诉你阿伏伽德罗常数\n2. 对于给定的物质的量，计算粒子数\n3. 对于给定的物质的量和摩尔质量，计算质量\n4. 告诉你标准状况下，气体摩尔体积\n5. 对于给定的物质的量，计算标准状况下，气体的体积\n6. 退出程序" << endl;
	while (true)
	{
		int choice;
		cout << "请输入你的选择：";
		cin >> choice;
		double n, m, V;
		switch (choice)
		{
		case 1:
			cout << "阿伏伽德罗常数是6.02e23/mol" << endl;
			break;
		case 2:
			cout << "请输入物质的量：";
			cin >> n;
			cout << "粒子数：" << n * 6.02e23 << endl;
			break;
		case 3:
			cout << "请输入物质的量：";
			cin >> n;
			cout << "请输入摩尔质量：";
			cin >> m;
			cout << "质量：" << n * m << endl;
			break;
		case 4:
			cout << "标准状况下，气体的摩尔体积：22.4L/mol" << endl;
			break;
		case 5:
			cout << "请输入物质的量：";
			cin >> n;
			cout << "请输入摩尔体积：";
			cin >> V;
			cout << "体积：" << n * V << endl;
			break;
		case 6:
			return 0;
		default:
			cout << "输入错误，请重新输入！" << endl;
			break;
		}
	}
}

