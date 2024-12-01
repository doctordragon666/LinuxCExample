#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>

struct Conn_stat
{
    int count;
    char ip[64];
};

int main()
{
    // 创建共享内存
    int shmid = shmget((key_t)1234, sizeof(struct Conn_stat), 0666 | IPC_CREAT);
    // 将共享内存连接到当前进程的地址空间
    void *shm = shmat(shmid, NULL, 0); // 连接到共享内存
    printf("\nMemory attached at %p\n", shm);

    struct Conn_stat *stat = (struct Conn_stat *)shm; // 转化为连接内存
    for (int i = 0; i < 10; i++)
    {
        printf("ip = %s ,count: %d\t\t\n", stat->ip, stat->count);
        sleep(1);
    }

    shmdt(shm);                 // 把共享内存从当前进程中分离
    shmctl(shmid, IPC_RMID, NULL); // 删除共享内存

    return 0;
}
