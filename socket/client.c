#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

char ch = 'A';

int main()
{
    int client_sockfd = socket(AF_INET, SOCK_STREAM, 0); // 建立客户端socket

    struct sockaddr_in address; // 服务器端网络地址结构体
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.127.130");
    address.sin_port = htons(9000);
    size_t len = sizeof(address);
    connect(client_sockfd, (struct sockaddr *)&address, len);
    
    // 第一次读写
    write(client_sockfd, &ch, 1);
    read(client_sockfd, &ch, 1);
    printf("the first time: char from server = %c\n", ch);
    sleep(3);

    // 第二次读写
    write(client_sockfd, &ch, 1);
    read(client_sockfd, &ch, 1);
    printf("the second time: char from server = %c\n", ch);

    close(client_sockfd);

    return 0;
}
