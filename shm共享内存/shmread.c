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
    struct Conn_stat *stat = NULL; //指向shm

    //创建共享内存
    int shmid; //共享内存标识符
    shmid = shmget((key_t)1234, sizeof(struct Conn_stat), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(0);
    }
    
    //将共享内存连接到当前进程的地址空间
    void *shm = NULL;         //分配的共享内存的原始首地址
    shm = shmat(shmid, 0, 0); //连接到共享内存
    if (shm == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(1);
    }
    printf("\nMemory attached at %p\n", shm);
    stat = (struct Conn_stat *)shm; //转化为连接内存

    int i = 0;
    while ((i++) < 10)
    {

        printf("ip = %s ,count: %d\t\t\n", stat->ip, stat->count);
        sleep(1);
    } //读取共享内存文件

    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(2);
    } //把共享内存从当前进程中分离

    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "shmctl(IPC_RMID) failed, reason: %s\n", strerror(errno));
        exit(3);
    } //删除共享内存

    return 0;
}
