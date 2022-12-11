#include <iostream>
#include <pthread.h>

/*
g++ rwlock2.cpp -o rwlock2.out -lpthread
readers & writer lock
*/

int main()
{
    // 読み書き属性オブジェクト
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr); /* 読み書き属性初期化 */
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NP);
    /* 読み書き属性設定
    int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr,int pref);
    pref:
        // 読み出し側に優先度を与える
        // 書き込み側が待っていたとしても、読み出し側が要求したロックを受け取ることを意味する。
        PTHREAD_RWLOCK_PREFER_READER_NP //(デフォルト)
        // 書き出し側に優先度を与える。
        PTHREAD_RWLOCK_PREFER_WRITER_NP
        //読み出しロックが再帰的に行われない限りは、 書き込み側の待ちを避けることができる。
        PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP
    */

    // readers /writer lock object 作成
    pthread_rwlock_t rwlock1 = PTHREAD_RWLOCK_INITIALIZER; // デフォ
    // attrに沿った設定で作成
    pthread_rwlock_t rwlock2;
    pthread_rwlock_init(&rwlock2, &attr /* 読み書き属性オブジェクト or null */);

    /* ⭐️読み取りのためのロック・オブジェクトのロック/アンロック */
    // int pthread_rwlock_rdlock(pthread_rwlock_t * rwlock1); // ロック取得(取得できるまで待つ)
    pthread_rwlock_rdlock(&rwlock1); // ロック取得(取得できるまで待つ)
    // int pthread_rwlock_unlock(pthread_rwlock_t * rwlock1); //
    pthread_rwlock_unlock(&rwlock1); //

    /* int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock1) // readロック取得(待たない)
        サブルーチンと同じように読み取りロックを適用します。
       ただし、何らかのスレッドが rwlock オブジェクトに対する書き込みロックを保持している場合、
       または rwlock オブジェクトでブロックされているライターがある場合、このサブルーチンは失敗します。
       つまり、書き込みlock中 or 書き込みlock取得待ちの時は失敗する。(読み込みlock中は大丈夫)
    */

    /* ⭐️書き込みのためのロック・オブジェクトのロック */
    // int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock1)　//ロック取得(取得できるまで待つ)
    // int pthread_rwlock_unlock(pthread_rwlock_t *rwlock1)

    /* int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock1) // writeロック
    // 何らかのスレッドが読み取りまたは書き込みのために、rwlock を現在保持している場合、 この関数は失敗します。
    // つまり、読み書き中は失敗する。
    */

    /* 読み書きの優先度とかのスケジューリングに使う */
    // pthread_rwlock_attr_setfavorwriters_np()

    /* ロックオブジェクトの破棄 */
    // pthread_rwlock_destroy(pthread_rwlock_t * rwlock1);
    pthread_rwlock_destroy(&rwlock1);

    return 0;
}