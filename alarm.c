//5秒后发信号触发

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

int wakeflag = 0;

void wake_handle(int sig) 
{
	wakeflag = 1;
}

int main(void) 
{
	int ret;
	
	struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = wake_handle;
	sigemptyset(&act.sa_mask);
	sigaction(SIGALRM, &act, 0);
	
	printf("time =%ld\n", time((time_t*)0));

	ret = alarm(5);//5秒后发送信号
	if (ret == -1) {
		printf("alarm error!\n");
		exit(1);
	}

	//挂起当前进程，直到收到任意一个信号
	pause();

	if (wakeflag) {
		printf("wake up, time =%ld\n", time((time_t*)0));
	}

	return 0;
}
