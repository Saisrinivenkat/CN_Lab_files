//
// Created by saisrini on 08/10/21.
//
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 800
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, MAX);
        read(sockfd, buff, sizeof(buff));
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        write(sockfd, buff, sizeof(buff));
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("SERVER EXIT...\n");
            break;
        }
    }
}
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("SOCKET CREATION FAILED...\n");
        exit(0);
    }
    else
        printf("SOCKET SUCCESSFULLY CREATED..\n");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("SOCKET BIND FAILED...\n");
        exit(0);
    }
    else
        printf("SOCKET SUCCESSFULLY BINDED..\n");

    if ((listen(sockfd, 5)) != 0)
    {
        printf("LISTEN FAILED...\n");
        exit(0);
    }
    else
        printf("SERVER LISTENING..\n");

    socklen_t len = sizeof(cli);
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0)
    {
        printf("SERVER ACCEPT FAILED...\n");
        exit(0);
    }
    else
        printf("SERVER ACCEPTED THE CLIENT...\n");

    func(connfd);
    close(sockfd);
}
