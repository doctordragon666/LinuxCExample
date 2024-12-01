/*************************************************
Date: 2024年12月1日10点59分
Description: 用pool来创建一个IO复用的服务器来读取
client客户端的内容。
*************************************************/

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <stdbool.h>

nfds_t NFD = 100;
char c = '\0';

int main()
{
    /* 建立标准服务器 */
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9000);
    int addr_size = sizeof(server_address);

    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // 建立服务器端socket

    bind(server_sockfd, (struct sockaddr *)&server_address, addr_size);
    listen(server_sockfd, 5); // 监听队列最多容纳5个

    // 初始化poll库
    struct pollfd fds[NFD];
    fds[server_sockfd].fd = server_sockfd;
    fds[server_sockfd].events = POLLIN;
    fds[server_sockfd].revents = 0;

    while (true)
    {
        if (poll(fds, NFD, 1000) < 0)
        {
            perror("server_pool");
            exit(1);
        }
        for (int i = 0; i < NFD; i++)
        {

            int cur_fd = fds[i].fd;
            if (cur_fd == server_sockfd && fds[i].revents == POLLIN)
            {
                // 建立连接
                int client_fd = accept(server_sockfd, (struct sockaddr *)&client_address, &addr_size);
                fds[client_fd].fd = client_fd;
                fds[client_fd].events = POLLIN;
                fds[client_fd].revents = 0;

                printf("adding client on port %d\n", client_address.sin_port);
            }
            else if (fds[i].revents & POLLIN)
            {
                if (read(cur_fd, &c, 1) == 0)
                {
                    close(cur_fd);
                    memset(&fds[i], 0, sizeof(struct pollfd));
                    printf("removing client on fd %d\n", cur_fd);
                    continue;
                }
                fds[i].events = POLLOUT;
            }
            else if (fds[i].revents & POLLOUT)
            {
                sleep(3);
                c++;
                write(cur_fd, &c, 1);
                fds[i].events = POLLIN;
                printf("serving client on fd %d, read: %c\n", cur_fd, c);
            }
        }
    }

    return 0;
}
