#include <iostream>
#include <stdio.h>

// FILE *popen(const char *command, const char *type);
// int pclose(FILE *stream);

int main()
{
    // ここでコマンドが実行される
    // "bash -c 'pwd -l'" /* -lが余計で失敗するパターン */
    FILE *fp;
    fp = popen("bash -c 'pwd -l'", "r");
    char buf[256] = {};

    // 標準出力取得
    while (fgets(buf, 256, fp))
        fputs(buf, stdout);

    // ここで返り値が取得できる。
    int ret = pclose(fp);
    printf("ret: %d\n", ret);
    return 0;
}