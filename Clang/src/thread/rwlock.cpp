#include <iostream>
#include <thread>
#include <shared_mutex>
#include <chrono>

std::mutex print_mtx;
void print_value(int x)
{
    std::lock_guard<std::mutex> lock(print_mtx);
    std::cout << x << std::endl;
}

class X
{
    std::shared_mutex mtx_;
    int count_ = 0;

public:
    // 書き込み側：カウンタを加算する
    void writer()
    {
        std::lock_guard<std::shared_mutex> lock(mtx_);
        ++count_;
    }

    // 読み込み側：カウンタの値を読む
    void reader()
    {
        int local_count;
        {
            std::shared_lock<std::shared_mutex> lock(mtx_);
            local_count = count_;
        } // 共有ロックをここで手放す
        print_value(local_count);
    }
};

X obj;
void writer_thread()
{
    for (int i = 0; i < 3; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        obj.writer();
    }
}

void reader_thread()
{
    for (int i = 0; i < 10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        obj.reader();
    }
}

int main()
{
    // 書き込みユーザー1人
    // 読み込みユーザー3人
    std::thread writer1(writer_thread);
    std::thread reader1(reader_thread);
    std::thread reader2(reader_thread);
    std::thread reader3(reader_thread);

    writer1.join();
    reader1.join();
    reader2.join();
    reader3.join();
}