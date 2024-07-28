#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

int test(int a[], int b[])
{
    int scorea = 0;
    for (int i = 3;i >= 0;i--)
    {
        int randa = rand() % (i + 1);
        int randb = rand() % (i + 1);
        if (a[randa] > b[randb])
        {
            scorea++;
        }
        a[randa] = 0;
        b[randb] = 0;
        for (int j = randa; j < 2 + i; j++)
        {
            a[j] = a[j + 1];
        }
        for (int j = randb; j < 2 + i; j++)
        {
            b[j] = b[j + 1];
        }
    }
    if (scorea >= 2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int main() {
    srand(time(NULL));
    int testtimes = 1000000;
    int x = 0;
    for (int i = 0; i < testtimes; i++)
    {
        x += test(new int[4] { 1,3,5,7 }, new int[4] { 2,4,6,8 });
    }
    cout << "赢的概率: " << x / (float)testtimes << endl;
    return 0;
}