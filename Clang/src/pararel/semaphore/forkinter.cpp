#include "forkinter.h"

/* セマフォKeyの確保 */
const key_t key = 112;
int sem_flags = 0666;

// セマフォ操作用
sembuf operations[1];

int CreateSemaphore(int key, int sem_flgs, semun *initOpe)
{
    int sem_id = semget(key, 1, sem_flags | IPC_CREAT);
    if (-1 == sem_id)
    {
        std::cerr << "Failed to acquire semapore" << std::endl;
        return EXIT_FAILURE;
    }

    if (initOpe != nullptr)
    {
        semctl(sem_id, 0, SETALL, *initOpe);
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
    /* セマフォの初期化(lock状態) */
    union semun argument;
    unsigned short values[1] = {LOCK};
    argument.array = values;

    int sem_id = CreateSemaphore(
        key, sem_flags | IPC_CREAT, &argument);

    // セマフォ操作用共用変数
    operations[0].sem_num = 0;
    operations[0].sem_op = UNLOCK;
    operations[0].sem_flg = SEM_UNDO;

    int nPid = fork();
    if (nPid < 0)
    { // fork失敗
        perror("fork failure");
        exit(-1);
    }

    // 親/子プロセスで分岐する
    if (nPid == 0) // child process
    {
        // 親待機状態になるまでスリープ
        sleep(2);
        ChildProcess(sem_id, nPid);
    }
    else // parent process
    {
        // 子プロセスがunlockするまで待機
        ParentProcess(sem_id, nPid);
        sleep(1);

        // semaphore削除
        DiscardSemaphore(sem_id);
        system("ipcs -s");
    }
    return 0;
}

void ParentProcess(int sem_id, int Pid)
{
    /* プロセスBの実行を待つ */
    std::cout << "ParentProcess:[" << Pid << "]Waiting for post operation..." << std::endl;
    operations[0].sem_op = WAIT;
    semop(sem_id, operations, 1);
    sleep(1);
    std::cout << "ParentProcess:[" << Pid << "] semaphore wait finished" << std::endl;
}
void ChildProcess(int sem_id, int Pid)
{
    std::cout << "ChildProcess[" << Pid << "] unlock it" << std::endl;
    /* セマフォにポスト */
    operations[0].sem_op = UNLOCK;
    semop(sem_id, operations, 1);
    std::cout << "ChildProcess[" << Pid << "] unlocked it" << std::endl;
}
