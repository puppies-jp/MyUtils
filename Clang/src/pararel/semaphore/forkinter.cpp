#include "forkinter.h"

int CreateSemaphore(int key, int sem_flgs, semun *initOpe = nullptr)
{
    int sem_id = semget(key, 1, sem_flags | IPC_CREAT);
    if (-1 == sem_id)
    {
        std::cerr << "Failed to acquire semapore" << std::endl;
        return EXIT_FAILURE;
    }

    if (initOpe != nullptr)
    {
        semctl(sem_id, 0, SETALL, initOpe);
    }
    return sem_id;
}

//セマフォ破棄
void DiscardSemaphore(int sem_id)
{
    auto result = semctl(sem_id, 0, IPC_RMID, 0);
    if (result == -1)
    {
        perror("semctl(del) failure");
        exit(-1);
    }
}

void ParentProcess(int sem_id, int Pid);
void ChildProcess(int sem_id, int Pid);

int main()
{
    // int sem_id = CreateSemaphore(key);
    int nPid = fork();
    if (nPid < 0)
    { // fork失敗
        perror("fork failure");
        exit(-1);
    }
    else if (nPid == 0) // child process
    {
        sleep(1); // 親のsemaphore作成待ちスリープ
        int sem_id = CreateSemaphore(key, sem_flags);
        ChildProcess(sem_id, nPid);
    }
    else // parent process
    {
        /* セマフォの初期化 */
        union semun argument;
        unsigned short values[1] = {1};
        argument.array = values;

        int sem_id = CreateSemaphore(
            key, sem_flags | IPC_CREAT, &argument);
        ParentProcess(sem_id, nPid);
        system("ipcs -s");
        sleep(10);
        DiscardSemaphore(sem_id);
    }
    return 0;
}

void ParentProcess(int sem_id, int Pid)
{
    /* プロセスBの実行を待つ */
    std::cout << "Waiting for post operation..." << std::endl;
    sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = WAIT;
    operations[0].sem_flg = SEM_UNDO;
    std::cout << "is this block?" << std::endl;
    semop(sem_id, operations, 1);
    std::cout << "[" << Pid << "] nop" << std::endl;
}
void ChildProcess(int sem_id, int Pid)
{
    std::cout << "[" << Pid << "] unlock it" << std::endl;
    sleep(1);
    /* セマフォにポスト */
    sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = UNLOCK;
    operations[0].sem_flg = SEM_UNDO;
    semop(sem_id, operations, 1);
    std::cout << "[" << Pid << "] unlocked it" << std::endl;
}
