//循环等待用户输入字符串，每收到一个字符串，就把它发送给msg_recv,直到用户输入exit


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_SIZE 80

struct my_msg_st {
	long int msg_type;
	char msg[MSG_SIZE];
};//定义相同的共同的信息结构

int main(void)
{
	int msgid;
	int ret;
	struct my_msg_st msg;

	msgid = msgget((key_t)1235, 0666|IPC_CREAT);
	if (msgid == -1) {
		printf("msgget failed!\n");
		exit(1);
	}//创建消息队列

	while(1) {
		fgets(msg.msg, sizeof(msg.msg), stdin);
			
		msg.msg_type = 1;	
		ret = msgsnd(msgid, &msg, MSG_SIZE, 0);
		if (ret == -1) {
			printf("msgsnd failed!\n");
			exit(1);
		}

		if (strncmp(msg.msg, "exit", 4) == 0) {
			break;
		}
	}

	return 0;
}
