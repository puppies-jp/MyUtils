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
