/*************************************************
Date: 2024年12月1日10点59分
Description:创建一个子进程,
子进程每1秒输出字符串“child process work!",
父进程等待用户输入,
如果用户按下字符A, 则向子进程发信号SIGUSR1, 
子进程的输出字符串改为大写;
如果用户按下字符a, 则向子进程发信号SIGUSR2, 
子进程的输出字符串改为小写
*************************************************/

#define _XOPEN_SOURCE // 在Linux桌面系统中没有这个问题
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

int workflag = 0;

void work_up_handle(int sig)
{
    workflag = 1;
    printf("change to upper\n");
    
}

void work_down_handle(int sig)
{
    workflag = 0;
    printf("change to lower\n");
}

int main(void)
{
    pid_t pd = fork();
    if (pd == -1)
    {
        printf("fork error!\n");
        exit(-1);
    }
    else if (pd == 0)
    {
        char *msg;
        struct sigaction act;
        sigaction(SIGUSR1, NULL, &act);
        act.sa_handler = work_up_handle;
        sigaction(SIGUSR1, &act, NULL);

        act.sa_handler = work_down_handle;
        sigaction(SIGUSR2, &act, NULL);

        while (1)
        {
            if (!workflag)
            {
                msg = "child process work!";
            }
            else
            {
                msg = "CHILD PROCESS WORK!";
            }
            printf("%s\n", msg);
            
            sleep(1);
        }
        return 0;
    }
    else
    {
        while (1)
        {
            char c = getchar();
            if (c == 'A')
            {
                kill(pd, SIGUSR1);
            }
            else if (c == 'a')
            {
                kill(pd, SIGUSR2);
            }
            else if (c == '\n')
            {
                continue;
            }
            else
            {
                kill(pd, SIGINT);
                break;
            }
        }
        exit(-1);
    }

    return 0;
}
