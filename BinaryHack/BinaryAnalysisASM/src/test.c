#include <stdio.h>

int f2(int x, int y)
{
    int z = x + y;
    return 0;
}

int f1(int x, int y)
{
    int z =f2(x, y);
    return 0;
}

int main()
{
    int x = f1(1, 3);
    return 0;
}
