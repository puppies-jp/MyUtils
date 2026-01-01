#include "SimpleThread.h"

// thread停止用
// int pthread_kill(pthread_t thread, int sig);

/* こんな感じでビルドできる
 * g++ SimpleThread.cpp -o main.out -lpthread -std=c++17
 */

void test1(void *ptr)
{
    printf("TEST setted function args");
}

lThreads::lThreads(std::string theadName)
{
    static int counter = 1;
    m_threadName = "[" + theadName + "(" + std::to_string(counter) + ")" + "]";
    m_thread = std::thread(mainJob, this);
    counter++;
}

void lThreads::threadStop()
{
    threadState = ThreadState::Finish;
    m_thread.join();
}

void lThreads::setState(ThreadState state)
{
    threadState = state;
}

void lThreads::setFunc(AsyncTask *src)
{
    task = src;
}

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
            TaskFunc threadFunc = self->task->func;
            threadFunc(self->task->args);
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