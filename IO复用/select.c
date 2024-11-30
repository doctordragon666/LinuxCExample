#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdbool.h>

/// @brief ch++返回客户端
/// @param fd 客户端句柄
/// @param ch 要发送的字符
void handle(int fd)
{
    char ch;
    read(fd, &ch, 1);
    sleep(5);
    printf("serving client on fd %d, read %c\n", fd, ch);
    ch++;
    write(fd, &ch, 1);
}

int main()
{
    /* 初始化服务器 */
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9000);
    int addr_size = sizeof(server_address);

    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0); // 建立服务器端socket
    bind(server_sockfd, (struct sockaddr *)&server_address, addr_size);
    listen(server_sockfd, 5); // 监听队列最多容纳5个

    /* 初始化fds数组 */
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds); // 将服务器端socket加入到集合中

    while (true)
    {
        int fd;
        int nread;
        printf("server waiting\n");

        /*无限期阻塞，并测试文件描述符变动 */
        // FD_SETSIZE：系统默认的最大文件描述符
        if (select(FD_SETSIZE, &readfds, NULL, NULL, NULL) < 1)
        {
            perror("server_select");
            exit(1);
        }

        /*扫描所有的文件描述符*/
        for (fd = 0; fd < FD_SETSIZE; fd++)
        {
            /*找到相关文件描述符*/
            if (FD_ISSET(fd, &readfds))
            {
                /*判断是否为服务器套接字，是则表示为客户请求连接。*/
                if (fd == server_sockfd)
                {
                    int client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &addr_size);
                    FD_SET(client_sockfd, &readfds); // 将客户端socket加入到集合中
                    printf("adding client on fd %d\n", client_sockfd);
                    continue;
                }
                /*客户端socket中有数据请求时*/

                ioctl(fd, FIONREAD, &nread); // 取得数据量交给nread
                /*客户数据请求完毕，关闭套接字，从集合中清除相应描述符 */
                if (nread == 0)
                {
                    close(fd);
                    FD_CLR(fd, &readfds); // 去掉关闭的fd
                    printf("removing client on fd %d\n", fd);
                    continue;
                }
                /*处理客户数据请求*/
                handle(fd);
            }
        }
    }

    return 0;
}
