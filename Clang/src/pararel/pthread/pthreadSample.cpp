#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_func(void *param);

int main(void)
{
    int threadNum = 3;
    pthread_t thread[threadNum];
    int args[threadNum];
    int ret = 0;

    // スレッドの作成
    // int pthread_create(pthread_t * thread, pthread_attr_t * attr,
    //                    void * (*start_routine)(void *), void * arg)
    // 第4引数に渡した値が、スレッド関数の引数に渡される
    for (int i = 0; i < threadNum; i++)
    {
        args[i] = i;
        ret = pthread_create(&thread[i], NULL, thread_func, (void *)&args[i]);
        if (ret != 0)
        {
            // pthread_createが失敗した場合は0以外の値を返す
            exit(1);
        }
    }

    // スレッドの終了を待機
    // int pthread_join(pthread_t th, void **thread_return)
    for (int i = 0; i < threadNum; i++)
    {
        ret = pthread_join(thread[i], NULL);
        if (ret != 0)
        {
            // pthread_joinが失敗した場合は0以外の値を返す
            exit(1);
        }

        /*
        // スレッドの終了を待たない
        // int pthread_detach(pthread_t th)
        ret = pthread_detach(thread);
        */
    }
    return EXIT_SUCCESS;
}

void *thread_func(void *param)
{
    int *num = (int *)param;
    // 非同期に実行する処理
    printf("Thread [%d]\n", *num);
    return nullptr;
}
