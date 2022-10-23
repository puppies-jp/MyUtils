#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <cstdlib>
#include <iostream>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

enum SEMAPHORE_OPERATION
{
    UNLOCK = -1,
    WAIT = 0,
    LOCK = 1,
};

int main()
{
    /* セマフォの確保 */
    const key_t key = 112;
    int sem_flags = 0666;
    int sem_id = semget(key, 1, sem_flags | IPC_CREAT);
    if (-1 == sem_id)
    {
        std::cerr << "Failed to acquire semapore" << std::endl;
        return EXIT_FAILURE;
    }

    /* セマフォの初期化 */
    union semun argument;
    unsigned short values[1] = {1};
    argument.array = values;
    semctl(sem_id, 0, SETALL, argument);

    /* プロセスBの実行を待つ */
    std::cout << "Waiting for post operation..." << std::endl;
    sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = WAIT;
    operations[0].sem_flg = SEM_UNDO;
    std::cout << "is this block?" << std::endl;
    semop(sem_id, operations, 1);

    /* セマフォの解放 */
    auto result = semctl(sem_id, 1, IPC_RMID, NULL);
    if (-1 == result)
    {
        std::cerr << "Failed to close semaphore" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
