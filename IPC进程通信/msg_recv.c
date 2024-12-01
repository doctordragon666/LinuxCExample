/*************************************************
Date: 2024年12月1日10点59分
Description:接受msg_send发过来的信息，并打印输出。
直到接受到exit。
*************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_SIZE 80

struct my_msg_st
{
	long int msg_type;
	char msg[MSG_SIZE];
};

int main(void)
{
	int msgid;
	int ret;
	struct my_msg_st msg;

	msgid = msgget((key_t)1235, 0666 | IPC_CREAT);
	if (msgid == -1)
	{
		printf("msgget failed!\n");
		exit(1);
	}

	int times = 1;
	while (times < 5)
	{
		msg.msg_type = 0;
		ret = msgrcv(msgid, &msg, MSG_SIZE, 0, 0);
		if (ret == -1)
		{
			printf("msgrcv failed!\n");
			times++;
			continue;
		}

		printf("received: %s\n", msg.msg);

		if (strncmp(msg.msg, "exit", 4) == 0)
		{
			break;
		}
	}

	ret = msgctl(msgid, IPC_RMID, 0);
	if (ret == -1)
	{
		printf("msgctl(IPC_RMID) failed!\n");
		exit(1);
	}

	return 0;
}
