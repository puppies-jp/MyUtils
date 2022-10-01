#include <thread>
#include <mutex>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
// thread停止用
// int pthread_kill(pthread_t thread, int sig);

/* こんな感じでビルドできる
 * g++ SimpleThread.cpp -o main.out -lpthread -std=c++17
 */

enum class ThreadState
{
    Inited = 0,
    WakeUp = 1, // 割り当てたタスクの開始
    Stop = 2,   // タスクの停止
    Finish = 3, // thread終了
};

class lThreads
{
private:
    std::thread m_thread;
    std::string m_threadName = "";
    static void mainJob(lThreads *self);

public:
    ThreadState threadState =
        ThreadState::Inited; //初期状態
    unsigned int interval = 5;
    void threadStop()
    {
        threadState = ThreadState::Finish;
        m_thread.join();
    }
    lThreads(std::string theadName)
    {
        m_threadName = theadName;
        m_thread = std::thread(mainJob, this);
    };

    ~lThreads()
    {
        // pthread_kill(m_thread, SIGTERM);
        if (threadState != ThreadState::Finish)
            threadStop();
    };
};

void lThreads::mainJob(lThreads *self)
{
    while (1)
    {
        ThreadState _threadState = self->threadState;
        if (_threadState == ThreadState::Finish)
        {
        }
        if (_threadState != ThreadState::WakeUp)
        {
            std::cout << self->m_threadName << ": Sleep 10s" << std::endl;
            sleep(self->interval);
        }
        else
        {
        }
    }
}

int main()
{
    lThreads t1[3]{
        std::string("Thread1"),
        std::string("Thread2"),
        std::string("Thread3")};
    sleep(30);

    return 0;
}