#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int memId = 0x33;
const int shared_segment_size = 0x6400;
const std::string filePath = "./key_data.dat";

void DeleteSharedMemory(int segmentId, char *shared_memory, bool isParenet = false);
int CreateSegmentKey(int memId, std::string path);

int main()
{
    int key = CreateSegmentKey(memId, filePath);
    int nPid = fork();
    if (nPid < 0)
    { // fork失敗
        perror("fork failure");
        exit(-1);
    }
    else if (nPid == 0) // child process
    {
        sleep(1);
        /* 共有メモリにアタッチ */
        const auto segment_id = shmget(key, 0, 0);
        const char *shared_memory = reinterpret_cast<char *>(shmat(segment_id, 0, 0));
        printf("shared memory attached at address %p\n", shared_memory);
        /* 共有メモリの読み込み */
        printf("%s\n", shared_memory);

        /* 共有メモリの書き込み */
        sprintf((char *)shared_memory, "[To Parenet from Child]Hello, world.");
        DeleteSharedMemory(segment_id, (char *)shared_memory);
    }
    else // parent process
    {
        /* セグメントの割当(領域割り当て) 1回だけでいいはず */
        const auto segment_id =
            shmget(key, shared_segment_size,
                   IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

        if (-1 == segment_id)
        {
            std::cerr << segment_id << " :Failed to acquire segment" << std::endl;
            return EXIT_FAILURE;
        }
        /* 共有メモリにアタッチ */
        char *const shared_memory = reinterpret_cast<char *>(shmat(segment_id, 0, 0));
        printf("shared memory attached at address %p\n", shared_memory);

        /* 共有メモリへの書込み */
        sprintf((char *)shared_memory, "[To Child from Parenet]Hello, world.");
        sleep(4);
        /* 共有メモリの読み込み */
        printf("%s\n", shared_memory);

        DeleteSharedMemory(segment_id, (char *)shared_memory, true);
    }
}

int CreateSegmentKey(int memId, std::string path)
{
    const auto key = ftok(path.c_str(), memId);
    if (-1 == key)
    {
        std::cerr << "Failed to acquire key" << std::endl;
        return EXIT_FAILURE;
    }
    return key;
}

void DeleteSharedMemory(int segmentId, char *shared_memory, bool isParenet)
{
    /* 共有メモリのデタッチ */
    shmdt(shared_memory);
    if (isParenet)
    {
        /* 共有メモリの解放 */
        shmctl(segmentId, IPC_RMID, 0);
    }
}