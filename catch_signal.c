#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/// @brief 捕捉信号
/// @param sig 信号
void catchhandle(int sig)
{
    printf("Catch a signal : %d\n", sig);
}

/// @brief 捕捉一次，然后恢复信号默认行为
/// @param sig 信号
void retreathandle(int sig)
{
    static int cnt = 0;
    printf("Catch a signal: %d\n",sig);
    signal(SIGINT, SIG_DFL);
}

void loophandle(int sig)
{
    printf("Catch a signal : %d\n", sig);
    for (int i = 0; i < 10; i++)
    {
        printf("%d->",i);
        sleep(1);
    }
    printf("Catch end. %d\n", sig);
}

void test_sigaction()
{
    struct sigaction act;
    act.sa_flags = 0;
    //act.sa_flags = SA_RESETHAND;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGUSR1);
    act.sa_handler = catchhandle;

    sigaction(SIGINT,&act,0);

    struct sigaction act2;
    act2.sa_handler = loophandle;
    sigemptyset(&act2.sa_mask);
    act2.sa_flags = 0;
    sigaction(SIGUSR1,&act,0);
}

void mutil_sigaction()
{
    struct sigaction act, act2;

	act.sa_handler = loophandle;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);//关联终端中断

	sigset_t proc_sig_msk, old_mask;
	sigemptyset(&proc_sig_msk);
	sigaddset(&proc_sig_msk, SIGINT);

	sigprocmask(SIG_BLOCK, &proc_sig_msk, &old_mask);//添加到老屏蔽集
	sleep(5);
	printf("had delete SIGINT from process sig mask\n");
	sigprocmask(SIG_UNBLOCK, &proc_sig_msk, &old_mask);//从新屏蔽中删除老屏蔽集中包含的信号
}

int main(void)
{
    //signal(SIGINT, catchhandle);

    //signal(SIGINT,retreathandle);//捕获到终止信号，执行retreat函数

    //test_sigaction();
    mutil_sigaction();

    while (1)
    {
        sleep(1);
    }
    return 0;
}
