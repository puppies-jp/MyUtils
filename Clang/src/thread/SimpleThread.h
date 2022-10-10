#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

enum class ThreadState
{
    Inited = 0,
    WakeUp = 1, // 割り当てたタスクの開始
    Stop = 2,   // タスクの停止
    Finish = 3, // thread終了
};

typedef void (*TaskFunc)(void *);

typedef struct _AsyncTask
{
    void *args = nullptr;
    TaskFunc func = nullptr;
} AsyncTask;

class lThreads
{
private:
    std::thread m_thread;
    std::string m_threadName = "";
    static void mainJob(lThreads *self);
    AsyncTask *task;

public:
    ThreadState threadState =
        ThreadState::Inited; //初期状態
    unsigned int interval = 5;
    void threadStop();

    lThreads(std::string theadName);
    // コピーコンストラクタ
    lThreads(const lThreads &src){};

    ~lThreads()
    {
        // pthread_kill(m_thread, SIGTERM);
        if (threadState != ThreadState::Finish)
            threadStop();
    };

    void setState(ThreadState state);
    void setFunc(AsyncTask *src);
};
