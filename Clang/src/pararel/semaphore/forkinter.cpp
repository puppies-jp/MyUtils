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

// セマフォ破棄
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
    /* セマフォの初期化 */
    union semun argument;
    unsigned short values[1] = {LOCK};
    argument.array = values;

    int sem_id = CreateSemaphore(
        key, sem_flags | IPC_CREAT, &argument);

    int nPid = fork();
    if (nPid < 0)
    { // fork失敗
        perror("fork failure");
        exit(-1);
    }
    else if (nPid == 0) // child process
    {
        // 親待機状態になるまでスリープ
        sleep(2);
        // int sem_id = CreateSemaphore(key, sem_flags);
        ChildProcess(sem_id, nPid);
    }
    else // parent process
    {
        // 子プロセスがunlockするまで待機
        ParentProcess(sem_id, nPid);
        sleep(4);
        DiscardSemaphore(sem_id);
        system("ipcs -s");
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
    semop(sem_id, operations, 1);
    sleep(1);
    std::cout << "[" << Pid << "] semaphore wait finished" << std::endl;
}
void ChildProcess(int sem_id, int Pid)
{
    std::cout << "[" << Pid << "] unlock it" << std::endl;
    /* セマフォにポスト */
    sembuf operations[1];
    operations[0].sem_num = 0;
    operations[0].sem_op = UNLOCK;
    operations[0].sem_flg = SEM_UNDO;
    sleep(1);
    semop(sem_id, operations, 1);
    sleep(1);
    std::cout << "[" << Pid << "] unlocked it" << std::endl;
}
