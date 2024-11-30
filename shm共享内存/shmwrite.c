#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

struct Conn_stat
{
    int count;
    char ip[64];
};

int main()
{
    //准备共享内存使用结构体
    struct Conn_stat stat = {0, "127.0.0.1"};

    //创建共享内存
    int shmid = shmget((key_t)1234, sizeof(struct Conn_stat), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(1);
    }

    //将共享内存连接到当前进程的地址空间
    void *shm = shmat(shmid, (void *)0, 0);
    if (shm == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(2);
    }
    printf("Memory attached at %p\n", shm);

    //设置共享内存
    struct Conn_stat *p = (struct Conn_stat *)shm;
    memcpy(p, &stat, sizeof(struct Conn_stat));

    int i = 0;
    while ((i++) < 30) //修改共享内存中写数据
    {
        p->count++;
        sleep(1);
    } //等待shmread读取

    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(3);
    } //把共享内存从当前进程中分离

    return 0;
}
