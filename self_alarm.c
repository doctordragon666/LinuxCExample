//创建一个子进程,子进程在5秒钟之后给父进程发送一个SIGALR,父进程收到SIGALRM信号之后，“闹铃”（用打印模拟） 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int wakeflag = 0;

void wake_handle(int sig)
{
    wakeflag = 1;
}

int main(void)
{
    pid_t pd;
    char c;

    pd = fork();
    if (pd == -1)
    {
        printf("fork error!\n");
        exit(1);
    }
    else if (pd == 0)
    {
        sleep(5);
        kill(getppid(), SIGALRM);
    }
    else
    {
        struct sigaction act;
        act.sa_handler = wake_handle;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);

        sigaction(SIGALRM, &act, 0);

        pause(); //把该进程挂起，直到收到任意一个信号

        if (wakeflag)
        {
            printf("Alarm clock work!!!\n");
        }
    }

    return 0;
}