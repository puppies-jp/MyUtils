# 並行コンピューティング技法

- ここではmulti thread/processのテクニック等についてまとめる

0. 並列化モデル
    - タスク分解(タスクの並行処理,task decomposition)
    - データ分解(データの並行処理,data decomposition)

1. thread間機能
[ミューテックス](#mutex)
[リードライトロック](#readers_writer_lock)
[条件変数](#condition)

2. process間機能

---

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

1. `shared_mutex`を使う
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

2. `pthread_rwlockattr_setkind_np`を使う
    - [pthread_rwlockattr_setkind_np使用例](https://linuxjm.osdn.jp/html/LDP_man-pages/man3/pthread_rwlockattr_setkind_np.3.html)

---

## <a name='condition'>条件変数</a>

条件変数は任意の条件式と結び付けられ、スレッドに状態(条件式の評価結果)の変化をスレッドに通知します。
スレッドは`pthread_cond_wait()`をコールし、条件変数の変更が通知されるまでブロックする。
状態が変化したら`pthread_cond_signal()`をコールし、条件変数でブロックしているスレッドの実行を再開される。状態変化を通知されたスレッドは、対応するmutexロックを獲得後、
`pthread_cond_wait()`からリターンし、結果に応じて再度条件変数を待つか処理を続けるか判断する。`(通知しても待っているスレッドが存在しなければ通知が破棄される)`。
また、`pthread_cond_broadcast()`は条件変数により待っている全スレッドの実行を再開させる。
