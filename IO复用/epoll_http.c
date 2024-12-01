#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdbool.h>

#define PORT 9000
#define EPOLL_SIZE 1024

int main()
{
    /* 建立标准服务器 */
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    int addr_size = sizeof(server_address);

    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // 建立服务器端socket
    int client_fd;

    bind(server_sockfd, (struct sockaddr *)&server_address, addr_size);
    listen(server_sockfd, 8); // 监听队列最多容纳5个

    /* 创建epoll句柄 */
    struct epoll_event ev;
    ev.data.fd = server_sockfd;
    ev.events = EPOLLIN | EPOLLET;

    struct epoll_event events[EPOLL_SIZE];
    int epoll_fd = epoll_create(EPOLL_SIZE);
    int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_sockfd, &ev);

    while (true)
    {
        int epoll_event_count = epoll_wait(epoll_fd, events, EPOLL_SIZE, 1000);
        if (epoll_event_count < 0)
        {
            perror("epoll");
            break;
        }
        for (int i = 0; i < epoll_event_count; i++)
        {
            int cur_fd = events[i].data.fd;
            if (cur_fd == server_sockfd && (events[i].events & EPOLLIN))
            {
                client_fd = accept(server_sockfd, (struct sockaddr *)&client_address, &addr_size);
                printf("client connection from: %s : % d(IP : port), "
                       "clientfd = %d \n",
                       inet_ntoa(client_address.sin_addr),
                       ntohs(client_address.sin_port),
                       client_fd);
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                continue;
            }
            char c;
            sleep(3);
            if (read(cur_fd, &c, 1) == 0)
            {
                printf("client close, client fd = %d\n", cur_fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, cur_fd, NULL);
                continue;
            } // 有bug，如果这个read出错了，没有及时终止！
            printf("server get %c from client %d\n", c++, cur_fd);
            write(cur_fd, &c, 1); // 应该让epoll取设置写事件的，而不是直接写，如果文件描述符关闭了就会出错
        }
    }

    close(server_sockfd);
    close(epoll_fd);

    return 0;
}