# RAII なロッククラスを実装する

- メリット

  - マルチスレッドで同期をとった処理をするためにロックする際に役立つ

## RAII なロッククラス(header ファイルだけで実装できる)

```cpp


#include <pthread.h>
#include <thread>
#include <mutex>

template <typename T>
class scoped_lock_
{
    T &m_;

public:
    explicit scoped_lock_(T &m) : m_(m) { m.lock(); };
    ~scoped_lock_() { m_.unlock(); };

private:
    void operator=(const scoped_lock_ &src) {}
    scoped_lock_(const scoped_lock_ &src) {}
};

struct static_mutex
{
    pthread_mutex_t m_;
    void lock() { pthread_mutex_lock(&m_); }
    void unlock() { pthread_mutex_unlock(&m_); }
    typedef scoped_lock_<static_mutex> scoped_lock;
};

namespace
{
    static_mutex m = {PTHREAD_MUTEX_INITIALIZER};
}

/* how to use lock */
/*
void need_to_sync()
{
    static_mutex::scoped_lock lk(::m);
}
*/
```

- 動作確認用テストソース

```cpp

#include <thread>
#include <mutex>
#include <iostream>

#include "RAIILockClass.hpp"

int g_i = 0;

void safe_increment()
{
    static_mutex::scoped_lock lk(::m);
    /**/
    ++g_i;

    std::cout << "g_i: " << g_i << "; in thread #"
              << std::this_thread::get_id();

    printf("\n");
    // printf("g_i: %d; in thread #\n", g_i);
    //  g_i_mutex is automatically released when lock
    //  goes out of scope
}

int main()
{
    std::cout << "g_i: " << g_i << "; in main()\n";

    std::thread t1(safe_increment);
    std::thread t2(safe_increment);
    std::thread t3(safe_increment);
    std::thread t4(safe_increment);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "g_i: " << g_i << "; in main()\n";
    assert(g_i == 4);
}
```
