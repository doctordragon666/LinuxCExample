/*************************************************
Date: 2024年12月4日15点42分
Description:使用管道进行通信，
父进程由用户输入一个字符，然后把该字符发给子进程，
子进程接收到以后，把该字符打印出来
*************************************************/

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>

int main()
{
    int pipfd[2];
    pipe(pipfd);
    pid_t pid = fork();
    switch (pid)
    {
    case 0:
        /* child process*/
        close(pipfd[1]);
        char c = -1;
        while (read(pipfd[0], &c, 1) <= 0);
        putchar(c),putchar('\n');
        close(pipfd[0]);
        break;
    case -1:
        perror("fork");
        break;
    default:
        /* parent process*/
        close(pipfd[0]);
        char wc = getchar();
        write(pipfd[1], &wc, 1);
        close(pipfd[1]);
        break;
    }
    return 0;
}