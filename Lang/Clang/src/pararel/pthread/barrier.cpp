#include <iostream>
#include <thread>
#include <shared_mutex>

/* c++系サンプル (std=c++20) */
//#include <barrier>
constexpr int NWORKERS = 2; // ワーカ数
constexpr int NPHASES = 3;  // フェーズ数

/* C pthreadでの場合 */
#include <pthread.h>
pthread_barrier_t barrier;     // バリアオブジェクト
pthread_barrierattr_t attr;    //バリア属性
unsigned count = NWORKERS + 1; // 並列数
int ret = pthread_barrier_init(&barrier, &attr, count);
//  int ret2 = pthread_barrier_wait(&barrier); wait(合流待ち)
/* ---------------- */

std::mutex cout_mtx; // 行単位cout出力用ミューテックス

// このプログラムでは (NWORKERS+1)*NPHASES = 9個のタスクを実行する。
// 同じフェーズに属するタスクは複数のスレッド上で同時並行に実行される一方、
// バリアにより異なるフェーズに属するタスクが同時実行されないことを保証する。
// ここでは周期的なFork-Joinモデルのタスク並行実行が行われる。
int main()
{
    // バリア同期: 初期カウント値=ワーカ数+1(メインスレッド)
    // std::barrier<> sync{NWORKERS + 1};
    std::string arr[3] = {"1", "2", "3"};

    // ワーカスレッド群をFire-and-Forget起動
    for (int id = 1; id <= NWORKERS; id++)
    {
        std::thread([&, id]
                    {
      for (int phase = 1; phase <= NPHASES; phase++) {
        { // ワーカスレッドのフェーズタスクを実行
          std::lock_guard lk{cout_mtx};
          std::cout << "Worker#" << id << " phase" << phase 
          << " lambda:"<< arr[id%NWORKERS]<< std::endl;
        }

        // 合流ポイント: メインスレッド＋他ワーカスレッドと同期
        //sync.arrive_and_wait();
        pthread_barrier_wait(&barrier);
      } })
            .detach();
    }

    // メインスレッド処理
    for (int phase = 1; phase <= NPHASES; phase++)
    {
        { // メインスレッドのフェーズタスクを実行
            std::lock_guard lk{cout_mtx};
            std::cout << "Main     " << phase << std::endl;
        }

        // 合流ポイント: 全ワーカスレッドと同期
        // sync.arrive_and_wait();
        pthread_barrier_wait(&barrier);
    }
}
