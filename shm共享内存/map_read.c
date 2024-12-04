/*************************************************
Date: 2024年12月1日10点59分
Description: 创建共享内存映射读取一个文件readfile.txt
*************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

struct Conn_stat
{
    int count;
    char ip[64];
};

int main(int argc, char *argv[]) // 这个进程创建映射区进行读
{
    if (argc != 2)
    {
        printf("Usage: %s  file.\n", argv[0]);
        exit(1);
    } // 获取映射文件名称

    int fd = open(argv[1], O_RDONLY, 0644);

    struct Conn_stat *p = (struct Conn_stat *)mmap(NULL, sizeof(struct Conn_stat), PROT_READ, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        exit(3);
    } // 映射失败

    for (int i = 0; i < 30; i++)
    {

        printf("ip = %s ,count: %d\t\t\n", p->ip, p->count);
        sleep(1);
    } // 检测p的更改,读取10次

    munmap(p, sizeof(struct Conn_stat));
    close(fd);

    return 0;
}
