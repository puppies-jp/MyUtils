# 共有メモリについてここでまとめる

- [サンプルソース](https://github.com/puppies-jp/MyUtils/tree/gh-pages/Clang/src/sharedMem)

- 送信側プロセス

```cpp
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>

int main()
{
    /* セグメントIDの生成 */
    const std::string file_path("./key_data.dat");
    const int id = 42;
    const auto key = ftok(file_path.c_str(), id);
    if (-1 == key)
    {
        std::cerr << "Failed to acquire key" << std::endl;
        return EXIT_FAILURE;
    }

    /* セグメントの割当 */
    const int shared_segment_size = 0x6400;
    const auto segment_id = shmget(key, shared_segment_size,
                                   IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (-1 == segment_id)
    {
        std::cerr << segment_id << " :Failed to acquire segment" << std::endl;
        return EXIT_FAILURE;
    }

    /* キーとセグメントIDの表示 */
    std::cout << "キー：" << key << std::endl;
    std::cout << "セグメントID:" << segment_id << std::endl;

    /* 共有メモリにアタッチ */
    char *const shared_memory = reinterpret_cast<char *>(shmat(segment_id, 0, 0));
    printf("shared memory attached at address %p\n", shared_memory);

    /* 共有メモリへの書込み */
    sprintf(shared_memory, "Hello, world.");

    std::cout << "Hit any key when ready to close shared memory" << std::endl;
    std::cin.get();

    /* 共有メモリへの再書込み */
    /* メモリを読み込む側もアタッチしていれば更新された状態で参照できる */
    sprintf(shared_memory, "Hello, world2.");
    std::cin.get();

    /* 共有メモリのデタッチ */
    shmdt(shared_memory);
    /* 共有メモリの解放 */
    shmctl(segment_id, IPC_RMID, 0);

    return EXIT_SUCCESS;
}
```

- 参照側プロセス

```cpp
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{
    /* セグメントIDの生成 */
    const std::string file_path("./key_data.dat");
    const int id = 42;
    const auto key = ftok(file_path.c_str(), id);
    if (-1 == key)
    {
        std::cerr << "Failed to acquire key" << std::endl;
        return EXIT_FAILURE;
    }

    /* 共有メモリにアタッチ */
    const auto segment_id = shmget(key, 0, 0);
    const char *const shared_memory = reinterpret_cast<char *>(shmat(segment_id, 0, 0));
    printf("shared memory attached at address %p\n", shared_memory);

    /* 共有メモリの読み込み */
    printf("%s\n", shared_memory);
    std::cin.get();
    /* メモリを読み込む側もアタッチしていれば更新された状態で参照できる */
    printf("%s\n", shared_memory);

    /* 共有メモリのデタッチ */
    shmdt(shared_memory);

    return EXIT_SUCCESS;
}
```