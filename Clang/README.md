# ここでは C/C++ 系の言語についてのあれこれをまとめる

- [Cmake について](Cmake)
- [コンストラクト/デストラクト](construct)
- [mutex の安全な運用について](mutex)
- [プリプロセッサ使い方 cheat メモ](preprocessor)
- [C++の安全なリソース解放について](SafeResource)
- [引数について](FunctionParameter)
- [inline調査](inline)
- [不本意ながらMFCというか、Windows関連](MFC)

---

- よく使うソース群
  - [メモリダンプ](#memory_dump)
  - [ネットワーク系](#network)
    - [サーバサイド](#server)
    - [クライアントサイド](#client)
  - [パスワード暗号化](#crypt)

- 小技系
  - [ダイレクトパラメータアクセス](#direct)
  - [ソースコードに直でブレークポイントを貼る](#breakPoint)

---

## <a name=memory_dump>メモリダンプ</a>

```c
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
```

---

## <a name=breakPoint>ブレークポイント</a>

```cpp
#include<signal.h>
raise(SIGTRAP)
```

## <a name=network>ネットワーク系</a>

- Coming soon

### <a name=server>サーバーサイド</a>

```c
```

### <a name=client>クライアントサイド</a>

```c
```

### <a name=direct>ダイレクトパラメータアクセス</a>

- printfに`$`を使ってやることで、引数の何番目の数字を使用するのかを直で指定できる

- 使い方はこんな感じ

```c
printf("%2$d",1,2,3);
```

- 出力例
  - ⚠️他の`%d`などのアクセス順に影響してない

```sh
(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# cat dyrectPrm.c 
#include<stdio.h>

int main()
{
    printf(
        "%d %d %1$d %d %d %d \n
        arg2: %2$d\n"
        ,1,2,3,4,5);
        return 0;
}

(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# ./a.out
1 2 1 3 4 5 
arg2: 2
(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# 
```

## <a name="crypt">password暗号化</a>

```c
#include <unistd.h>
#include <stdio.h>

/* 🌟コンパイルオプションにcryptをリンクすること
 * g++ crypt_test.cpp -o crypt.out -lcrypt
 */

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage %s <password> <salt>\n", argv[0]);
        return 1;
    }
    printf("password = '%s' , salt = '%s'\n", argv[1], argv[2]);
    printf("hash => '%s'\n", crypt(argv[1], argv[2]));
}
```