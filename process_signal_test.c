// 使用信号量实现对文件操作的互斥访问。
// 程序1，对test.txt写入学生记录信息10条
// 程序2，对test.txt写入教师记录信息10条
// 程序1和程序2并发执行

//使用信号量，执行多个进程，观察对临界区的访问
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

const int SEM_VALUE = 1;//信号量指定的初始值

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
#else
union semun
{
    int val;//setval命令要设置的值
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};
#endif

/// @brief 初始化信号量
/// @param semid 信号量标识符
/// @return 错误信息
static int sem_initial(int semid)
{
    int ret;

    union semun semun;
    semun.val = SEM_VALUE;
    ret = semctl(semid, 0, SETVAL, semun);
    if (ret == -1)
    {
        fprintf(stderr, "semctl failed!\n");
    }

    return ret;
}

/// @brief 信号量p操作
/// @param semid 
/// @return 
static int sem_p(int semid)
{
    int ret;

    struct sembuf sembuf;
    sembuf.sem_op = -1;//表示p操作
    sembuf.sem_num = 0;//编号，一个编号0
    sembuf.sem_flg = SEM_UNDO;//进程终止，不释放信号量
    ret = semop(semid, &sembuf, 1);
    if (ret == -1)
    {
        fprintf(stderr, "sem_p failed!\n");
    }

    return ret;
}

/// @brief 信号量v操作
/// @param semid 
/// @return 
static int sem_v(int semid)
{
    int ret;

    struct sembuf sembuf;
    sembuf.sem_op = 1;
    sembuf.sem_num = 0;
    sembuf.sem_flg = SEM_UNDO;
    ret = semop(semid, &sembuf, 1);
    if (ret == -1)
    {
        fprintf(stderr, "sem_v failed!\n");
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int i;
    int ret;
    int semid;

    /* 获取信号量 */
    semid = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    if (semid == -1)
    {
        printf("semget failed!\n");
        exit(1);
    }

    /* 初始化信号量 */
    ret = sem_initial(semid);
    if (ret == -1)
    {
        exit(1);
    }

    int pid = fork();
    for (i = 0; i < 5; i++)
    {

        if (sem_p(semid) == -1)
        {
            exit(1);
        }

        /* 模拟临界区----begin */
        FILE* data = fopen("test.txt","a+");
        if(pid == 0)
            fprintf(data,"child process%d\n",getpid());
        else
            fprintf(data,"parent process%d\n",getpid());
        fclose(data);
        /* 模拟临界区----end */

        if (sem_v(semid) == -1)
        {
            exit(1);
        }

        sleep(1);
    }

    /* 删除信号量 */
    semctl(semid, 0, IPC_RMID);

    return 0;
}