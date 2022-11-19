#include <stdio.h>

int main()
{
    int i;
    int a, b;
    a = 100;
    b = 500;
    int ret;

    __asm__(
        "movl %1,%0;\
        addl %2,%0;"
        : "=&r"(ret)
        : "r"(a), "r"(b));
    printf("ret=%d\n", ret);

    return 0;
}
