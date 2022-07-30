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
    printf("%s\n", shared_memory);

    /* 共有メモリのデタッチ */
    shmdt(shared_memory);

    return EXIT_SUCCESS;
}
