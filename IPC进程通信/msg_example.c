#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSG_SIZE 80

struct my_msg_st
{
	long int msg_type;
	char msg[MSG_SIZE];
};

/// @brief
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

	msg.msg_type = 1;
	strcpy(msg.msg, "Hello world!");
	ret = msgsnd(msgid, &msg, MSG_SIZE, 0);
	if (ret == -1)
	{
		printf("msgsnd failed!\n");
		exit(1);
	}
	sleep(10);

	struct my_msg_st tmp_msg;
	msgrcv(msgid, &tmp_msg, MSG_SIZE, 0, 0);
	printf("recv data %s\n",tmp_msg.msg);

	return 0;
}
