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
// tls_Check.cpp
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

### unwind,backtrace を使う

- `unwind`とは一般的にスタックの巻き戻し処理を意味する。

  - `libunwind` というライブラリを使用することで、実装ができる。
  - つまり、スタックフレームから `main -> fun1 -> func2 -> func3` と呼び出し、`func3` 内で巻き戻しを実行することで、`func2,func1` の return を通らずに main 関数に戻ることができたりする。

  - また、スタックフレームを辿るため、関数のバックトレースを作成する事もできる。

```cpp
//btrace.cpp
#include <cstddef>
#include <iostream>
#include <libunwind.h>

using namespace std;

// backtraceの出力を呼びたい箇所でこの関数を実行すると、取得できる。
void show_bachtrace()
{
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip, sp;
    char buf[4096];
    unsigned long offset;

    unw_getcontext(&uc);
    unw_init_local(&cursor, &uc);
    while (unw_step(&cursor) > 0)
    {
        unw_get_reg(&cursor, UNW_REG_IP, &ip);
        unw_get_reg(&cursor, UNW_REG_SP, &sp);
        unw_get_proc_name(&cursor, buf, 4095, &offset);
        printf("0x%8lx <%s+0x%lx>\n", (long)ip, buf, offset);
    }
}
```

### signal をハンドリングする

```cpp
// signal_handler.cpp
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#include "btrace.cpp"

#define ALT_STACK_SIZE (64 * 1024)

static sigjmp_buf return_point;
static void signal_handler(int sig, siginfo_t *sig_info, void *sig_data)
{
    printf("catch signal\n");
    if (sig == SIGSEGV)
    {
        show_bachtrace();
        siglongjmp(return_point, 1);
    }
}

static void meaningless_recursive_func()
{
    int buff[1024] = {};
    meaningless_recursive_func();
}

static void register_signalstack()
{
    stack_t newSS, oldSS;

    newSS.ss_sp = malloc(ALT_STACK_SIZE);
    newSS.ss_size = ALT_STACK_SIZE;
    newSS.ss_flags = 0;
    sigaltstack(&newSS, &oldSS);
}

int main(int argc, char **argv)
{
    struct sigaction newAct, oldAct;

    /* 代替シグナルスタック設定 */
    register_signalstack();

    /* signal handler set */
    sigemptyset(&newAct.sa_mask);
    sigaddset(&newAct.sa_mask, SIGSEGV);
    newAct.sa_sigaction = signal_handler;
    newAct.sa_flags = SA_SIGINFO | SA_RESTART | SA_ONSTACK;

    sigaction(SIGSEGV, &newAct, &oldAct);

    if (sigsetjmp(return_point, 1) == 0)
    {
        meaningless_recursive_func();
    }
    else
    {
        show_bachtrace();
        fprintf(stderr, "stack overflow error\n");
    }

    return 0;
}
```
