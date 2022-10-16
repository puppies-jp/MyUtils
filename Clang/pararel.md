# 並行コンピューティング技法

- ここではmulti thread/processのテクニック等についてまとめる

0. 並列化モデル
    - タスク分解(タスクの並行処理,task decomposition)
    - データ分解(データの並行処理,data decomposition)

1. thread間機能
[ミューテックス](#mutex)
[リードライトロック](#readers_writer_lock)
[条件変数](#condition)
[バリアオブジェクト](#barrier)
[volatile宣言について](#volatile)

2. process間機能

---

## <a name=pthread>pthreadについて</a>

🚨 `pthread_create`した後は必ず`join`,`detach`がいる。

- `pthread_create`でスレッドを作成した場合、
  `join`もしくは、`detach`しなければなりません。 pthread_detachは、メインスレッドからスレッドを切り離す機能を提供します。
  `pthread_create`で作ったスレッドが終了したとき、`join`で終了を回収せずに、`pthread_create`だけを呼び出して、`プログラムを動かしているとメモリリーク`します。

## <a name='mutex'>mutex</a>

言わずもがな排他機能付す数の`スレッド`から値を参照/更新するのを避け、1スレッドからのアクセスのみに制限する機構

```cpp
std::mutex mtx_;

// lockクラスでmutexをlockする(デストラクタでリリースされる)
std::lock_guard<std::mutex> lock(mtx_);
```

---

## <a name='readers_writer_lock'>リードライトロック</a>

複数のスレッドで値を参照するのは問題ない。逆に排他をかけてしまうとそこがボトルネックとなってしまう。他スレッドが更新することがあるならば排他が必須となる。
`参照回数が多く、更新回数が圧倒的に少ない共有変数`にはリードライトロック(readers/writer lock)がいい。
これは、スレッドが値を更新する際に、`他の参照スレッドの参照が終了し、更新が終了していることを保証する。`

- 実装に当たって

- `shared_mutex`を使う
  - `mutex`クラスに対し、`lock_guard`(書き込み用のロッククラス)/`shared_lock`(読み込み用ロッククラス)でロックすることでreaders/writer lockした動きとなる。
  - [shared mutex使用例](https://cpprefjp.github.io/reference/shared_mutex/shared_mutex.html)
  - 🚨このクラスは現状、`書き込みロック`と`読み込みロック`についての`スケジューリング戦略を規定せず、カスタマイズもできない。`そのため、書き込みロックがなかなか取得できない、読み込みロックがなかなか取得できないというスタベーションの問題が発生した場合に、ユーザーの状況に合わせて戦略を変更することができない。
    - `pthread_rwlockattr_setkind_np`を使うことで、対応することができる。

```cpp
std::shared_mutex mtx_;
// 書き込みロック(デストラクタでリリースされる)
std::lock_guard<std::shared_mutex> lock(mtx_);
// 読み込みロック(デストラクタでリリースされる)
std::shared_lock<std::shared_mutex> lock(mtx_); 
```

- `pthread_rwlockattr_setkind_np`を使う
  - [pthread_rwlockattr_setkind_np使用例](https://linuxjm.osdn.jp/html/LDP_man-pages/man3/pthread_rwlockattr_setkind_np.3.html)

```cpp

// 読み書き属性オブジェクト
pthread_rwlockattr_t attr;
pthread_rwlockattr_init(&attr); /* 読み書き属性初期化 */
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
pthread_rwlock_t rwlock1 = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t rwlock2;
pthread_rwlock_init(&rwlock2, &attr /* 読み書き属性オブジェクト or null */);

/* ロックオブジェクトの破棄 */
pthread_rwlock_destroy(pthread_rwlock_t *rwlock1)
```

```cpp
/* 読み書きの優先度とかのスケジューリングに使う */
pthread_rwlock_attr_setfavorwriters_np()
    

/* ⭐️読み取りのためのロック・オブジェクトのロック/アンロック */
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock1) //ロック取得(取得できるまで待つ)
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock1) //

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock1) // readロック取得(待たない)
/*
    サブルーチンと同じように読み取りロックを適用します。
    ただし、何らかのスレッドが rwlock オブジェクトに対する書き込みロックを保持している場合、
    または rwlock オブジェクトでブロックされているライターがある場合、このサブルーチンは失敗します。
    つまり、書き込みlock中 or 書き込みlock取得待ちの時は失敗する。(読み込みlock中は大丈夫)
*/
    
/* ⭐️書き込みのためのロック・オブジェクトのロック */
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock1)　//ロック取得(取得できるまで待つ)
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock1)

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock1) // writeロック
/* 
    何らかのスレッドが読み取りまたは書き込みのために、
    rwlock を現在保持している場合、 この関数は失敗します。
    つまり、読み書き中は失敗する。
*/
```

---

## <a name='condition'>条件変数</a>

条件変数は任意の条件式と結び付けられ、スレッドに状態(条件式の評価結果)の変化をスレッドに通知します。
スレッドは`pthread_cond_wait()`をコールし、条件変数の変更が通知されるまでブロックする。
状態が変化したら`pthread_cond_signal()`をコールし、条件変数でブロックしているスレッドの実行を再開される。状態変化を通知されたスレッドは、対応するmutexロックを獲得後、
`pthread_cond_wait()`からリターンし、結果に応じて再度条件変数を待つか処理を続けるか判断する。`(通知しても待っているスレッドが存在しなければ通知が破棄される)`。
また、`pthread_cond_broadcast()`は条件変数により待っている全スレッドの実行を再開させる。

## <a name='barrier'>バリアオブジェクト</a>

`バリアオブジェクト`とは
同期メカニズムの一つ、実行中の他のスレッドがバリアポイントに到達するまで、スレッドの実行を一時停止するもの。

> `C++20`からクラスが作成されてる。
それ以前だと`Pthread`で実装がいるかも

- [バリア同期(C++20)](https://cpprefjp.github.io/reference/barrier.html)

```cpp
#include <pthread.h>

pthread_barrier_t barrier;  // バリアオブジェクト
pthread_barrierattr_t attr; //バリア属性
unsigned count;             // 並列数
int ret = pthread_barrier_init(&barrier, &attr, count);


int ret = pthread_barrier_wait(&barrier);
```

---

## <a name=volatile>volatile宣言</a>

`volatile`とは`「揮発性の」`のこと、つまりすぐに変わってしまうという意味を持ちます。
マルチスレッドで値を書き換える可能性があることを示すため、コンパイラでの省略がされなくなる。
(処理系によっては正しく動かないのもあるらしいです。)

```cpp
volatile int flag = 0;
void worker() {
    // volatile宣言してないと、コンパイル時の最適化で
    // whileが全てtrueとなることがある。
    while( flag == 0){}
    std::cout << "worker!! \n";
}
```
