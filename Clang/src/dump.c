#include "stdio.h"

/*
引数のアドレスから16byte単位で改行し、16進数とasciiコード単位で出力する関数
*/
void dump(const unsigned char *data_buffer, const unsigned int length)
{
    unsigned char byte;
    unsigned int i, j;

    for (i = 0; i < length; i++)
    {
        byte = data_buffer[i];
        printf("%02X ", byte);

        // 16byte表示した or 16byte以下で末尾に到達した。
        if (((i % 16) == 15) || (i == length - 1))
        {
            for (j = 0; j < 15 - (i % 16); j++)
                printf("   "); // 不足分の埋め草を出力する。

            // asciiコードの文字列を出力する
            printf("| ");

            // 何文字表示したかを16byte単位で丸めてj -> i の値になるまでループ
            for (j = i - (i % 16); j <= i; j++)
            {
                byte = data_buffer[j];
                if ((byte > 31) && (byte < 127))
                    printf("%c", byte);
                else
                    printf(".");
            }
            printf("\n");
        }
    }
}

/* test用関数
int main()
{
    char *buffer = "this is test.\n Is this dumper right?";
    dump(buffer, 37);
    return 0;
}
*/