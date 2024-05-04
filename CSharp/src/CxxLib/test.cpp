#include <stdio.h>

extern "C"
{
    void test();
}

void test()
{
    printf("This is C++ Library!!\n");
}