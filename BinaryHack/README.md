# BinaryHacks

## ここでは BinaryHacks を読んだ内容をざっくりとまとめる

### ELF(Executable and Linking Format) とは

- 実行可能バイナリ、オブジェクトファイルのフォーマットを規定したもの。ELF フォーマットのファイルは先頭に ELF ヘッダがあり、その後にプログラムヘッダテーブル、セクションヘッダテーブルがある。

  - ヘッダの構造は elf.h にまとまっている。

- これを使うことで、binary ファイルの詳細が得られる。

### builtin 関数

- gcc には builtin で動作時の状況を知る関数などがある。

```cpp
void *__builtin_return_address(unsigned int LEVEL)
    関数のリターンアドレス(関数の呼び出しもとを返す関数。)
```

### TLS(thread local storage)を使う

gcc にはどのスレッドでも独立して同名の変数を使うことができる領域を作成、保持できる。その領域を TLS(thread local storage)という。

以下のように書くことで確認できる。

- 出力内容

```sh
user@MacBook BinaryHack % ./tlsTest.out
Thread 1 tls 1 global 2
Thread 0 tls 0 global 2
Thread 2 tls 2 global 2
```

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD 3

__thread long tls;
long global;

void *func(void *args)
{
    // 内部で定義した変数
    long num = (long)args;
    // tlsと指定した変数(ここはスレッド単位で設定されるのでパラで違う値となる。)
    tls = num;
    // global変数(ここを書き換えたらグローバルに全スレッドで書き換えられる。)
    global = num;

    sleep(1);
    printf("Thread %ld tls %ld global %ld \n",
           num, tls, global);

    return NULL;
}

int main()
{
    int ret;
    pthread_t thread[THREAD];
    long num;
    for (num = 0; num < THREAD; num++)
    {
        ret = pthread_create(&thread[num], NULL,
                             &func, (void *)num);
        if (ret)
        {
            printf("error pthread_create\n");
            exit(1);
        }
    }

    for (num = 0; num < THREAD; num++)
    {
        ret = pthread_join(thread[num], NULL);
        if (ret)
        {
            printf("error join\n");
            exit(1);
        }
    }
    exit(0);
}

```
