#include <thread>
#include <mutex>
#include <vector>
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

typedef void (*TaskFunc)(void *);

typedef struct _AsyncTask
{
    void *args = nullptr;
    TaskFunc func = nullptr;
} AsyncTask;

void test1(void *ptr)
{
    printf("TEST setted function args");
}

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
    void threadStop()
    {
        threadState = ThreadState::Finish;
        m_thread.join();
    }
    lThreads(std::string theadName)
    {
        static int counter = 1;
        m_threadName = "[" + theadName + "(" + std::to_string(counter) + ")" + "]";
        m_thread = std::thread(mainJob, this);
        counter++;
    };
    // コピーコンストラクタ
    lThreads(const lThreads &src){};

    ~lThreads()
    {
        // pthread_kill(m_thread, SIGTERM);
        if (threadState != ThreadState::Finish)
            threadStop();
    };

    void setState(ThreadState state)
    {
        threadState = state;
    }

    void setFunc(AsyncTask *src)
    {
        task = src;
    }
};

void lThreads::mainJob(lThreads *self)
{
    while (1)
    {
        ThreadState _threadState = self->threadState;
        if (_threadState == ThreadState::Finish)
        {
            break;
        }
        if (_threadState != ThreadState::WakeUp)
        {
        }
        else
        {
            self->task->func(self->task->args);
        }

        std::cout << self->m_threadName << ": Sleep " + std::to_string(self->interval) + "s" << std::endl;
        sleep(self->interval);
        continue;
    }
}

int main()
{
    lThreads Tasks[3]{
        {"Thread1"},
        {"Thread2"},
        {"Thread3"},
    };
    AsyncTask ag = {nullptr, test1};
    Tasks[0].setFunc(&ag);
    Tasks[0].setState(ThreadState::WakeUp);
    sleep(10);
    Tasks[1].setFunc(&ag);
    Tasks[1].setState(ThreadState::WakeUp);
    Tasks[0].setState(ThreadState::Inited);
    sleep(10);
    return 0;
}