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
[セマフォ](#semapho)  
[共有メモリ](#share_mem)

    - [参考](https://qiita.com/MoriokaReimen/items/5c4256ef620499a88bb3)

3. 実行効率計測ツール周り

- [Intel VTune Profiler](https://www.intel.com/content/www/us/en/develop/documentation/vtune-install-guide/top/linux.html)
  - インテルのプロセッサ周りの実行プロファイルを作成してくれる。
  - Linux,Windows,macでも動かせるらしい(ARM,AMDとかプロセッサ依存で可否が変わるのかな？(使うときに調査がいるかも))

---

## <a name=pthread>pthreadについて</a>

🚨 `pthread_create`した後は必ず`join`,`detach`がいる。

- `pthread_create`でスレッドを作成した場合、
  `join`もしくは、`detach`しなければなりません。 pthread_detachは、メインスレッドからスレッドを切り離す機能を提供します。
  `pthread_create`で作ったスレッドが終了したとき、`join`で終了を回収せずに、`pthread_create`だけを呼び出して、`プログラムを動かしているとメモリリーク`します。

[pthread Cサンプル](https://github.com/puppies-jp/MyUtils/blob/gh-pages/Clang/src/pararel/pthread/pthreadSample.cpp)  
[pthread std C++サンプル](https://github.com/puppies-jp/MyUtils/blob/gh-pages/Clang/src/pararel/pthread/SimpleThread.cpp)  

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
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock1) //ロック取得(取得できるまで待つ)
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

---

## <a name=semapho>セマフォの使い方</a>

セマフォとは。。。

コンピュータ上の共有資源について、利用可能な資源の数を指し示している。プログラムが資源を占有するときはセマフォの値から1を減じ、処理が終わって解放する際には1を加える。

OSの機能を使うため生成/削除を漏らさずに実行しないと前回実行したセマフォが残る可能性がある。

```sh
# 🌟セマフォの利用状況について以下コマンドで確認できる
ipcs -s

# 🌟セマフォの削除は以下
ipcrm -s <semid>
```

- [sampleスクリプト](https://github.com/puppies-jp/MyUtils/tree/gh-pages/Clang/src/pararel/semaphore)

### semget

```cpp
#include <sys/sem.h>

// 🌟既存セマフォ集合のセマフォIDあるいは、新規作成したセマフォ集合のセマフォIDを取得します。
// key:取得するセマフォ集合のキー  nsems:セマフォの数  semflg:オプション
int semget(key_t key, int nsems, int semflg);

// 作り方は2通り
// 1.第１引数のkeyにIPC_PRIVATEを指定します。
// 2. 第１引数のkeyにユニークな値を指定し、第３引数のsemflgにIPC_CREATを指定します。
// semflgにIPC_CREATとIPC_EXCLを指定すると、keyに対するセマフォ集合が既に存在していた場合にエラーになります。
// 第３引数のsemflgの下位９ビットは、そのセマフォの所有者、グループ、他人に対するアクセス許可の定義として使用します。
const key_t key = 112;int sem_flags = 0666;
int sem_id = semget(key, 1, sem_flags | IPC_CREAT);
```

### semctl

```cpp
// semid で指定された  System V セマフォ集合 (semaphore set) またはセマフォ集合の 
// semnun 番目のセマフォに対して、 cmd で指定された制御操作を行なう (集合内のセマフォの番号は 0 から始まる)。 
int semctl(
  int semid,  /* セマフォ集合のId */
  int semnum, /* 何番目のセマフォか */
  int cmd,    /* 制御操作 */
  ...
  );

union semun
{
    int val;/* SETVAL の値 */ 
    struct semid_ds *buf;/* IPC_STAT,IPC_SET 用のバッファー */ 
    unsigned short int *array;/* GETALL,SETALL 用の配列 */ 
    struct seminfo *__buf;
};

/* セマフォの初期化 */
union semun argument;
unsigned short values[1] = {1}; // ここの配列で設定した値が初期値で設定される
argument.array = values;
semctl(sem_id, 0, SETALL, argument);

/* セマフォの解放 */
int result = semctl(sem_id, 1, IPC_RMID, NULL);    
```

```cpp
// 🌟 セマフォ操作
int semop(
  int semid, // セマフォId
  struct sembuf *sops, // セマフォ操作配列
  size_t nsops // セマフォ操作構造体の数
  );

/* sembuf.sem_op の操作 */
enum SEMAPHORE_OPERATION
{
  // 🌟セマフォ値（semval）が指定した値の絶対値以上の場合は、セマフォ値から指定した値の絶対値を減算し、プロセスを停止状態にします。
  // セマフォ値が指定した値の絶対値以上になると、プロセスは目覚めます。
  UNLOCK = -1,
  // 🌟sem_num=0 でセマフォ値（semval）が０になるまで待ち（プロセスの停止）ます。（ロックの操作）
  WAIT = 0,
  // 🌟指定した値(sem_num>0)をセマフォ値（semval）に加算します。
  // この操作は必ず実行でき、プロセスの停止状態は起こりません。（アンロックの操作）
  LOCK = 1,
};

// 🌟使用例
sembuf operations[1];
operations[0].sem_num = 0;
operations[0].sem_op = WAIT;
operations[0].sem_flg = SEM_UNDO;
// sem_flgには休眠状態（停止）を避ける為のIPC_NOWAITと、シグナル等でプロセスが終了した場合に、セマフォ値を元に戻す為のSEM_UNDOが設定できます。どちらも指定しない場合は０を指定します。

semop(sem_id, operations, sizeof(operations)/sizeof(sembuf));
```

### <a name=share_mem>共有メモリ</a>

```bash
# IPC(inter process communication)
# 🌟共有メモリの確認
ipcs 

# 🌟セマフォとかがうざい時は `-m`オプションで共有メモリだけ表示できる
ipcs -m

# 出力はこんな感じ

------ 共有メモリセグメント --------
キー     shmid      所有者  権限     バイト  nattch     状態      
0x00000000 0          root       777        2560       2                       
0x00000000 1          root       777        2048000    2                       
0x00000000 2          root       777        2048       2                       

# 🌟共有メモリの削除
ipcrm shm <shmid>
```

- [とりあえずここ参照](https://puppies-jp.github.io/MyUtils/Clang/src/sharedMem/)

```cpp
```
