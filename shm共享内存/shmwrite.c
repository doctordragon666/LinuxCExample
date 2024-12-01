/*************************************************
Date: 2024年12月1日10点59分
Description: 创建共享内存以供shmread读取内容
*************************************************/

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
    // 准备共享内存使用结构体
    struct Conn_stat stat = {0, "127.0.0.1"};

    // 创建共享内存
    int shmid = shmget((key_t)1234, sizeof(struct Conn_stat), 0666 | IPC_CREAT);

    // 将共享内存连接到当前进程的地址空间
    void *shm = shmat(shmid, NULL, 0);
    printf("Memory attached at %p\n", shm);

    // 设置共享内存
    struct Conn_stat *p = (struct Conn_stat *)shm;
    memcpy(p, &stat, sizeof(struct Conn_stat));

    for (int i = 0; i < 30; i++)
    {
        p->count++;
        sleep(1);
    }

    printf("memery has used");
    shmdt(shm);                 // 把共享内存从当前进程中分离
    shmctl(shmid, IPC_RMID, NULL); // 删除共享内存

    return 0;
}
