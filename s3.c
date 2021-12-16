//
// Created by saisrini on 08/10/21.
//

#include<sys/types.h>
#include<sys/socket.h>
#include"time.h"
#include<stdio.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>
#include <arpa/inet.h>
int main()
{
    int b,sockfd,connfd,l,n,len;
    int choice;
    char str[100];
    time_t tick;
    int op1,op2,result;

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))>0)
        printf("SOCKET CREATED SUCCESSFULLY\n");

    struct sockaddr_in servaddr,clientaddr;

    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=8080;

    if((bind(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)))==0)
        printf("BIND SUCCESSFUL\n");

    if((listen(sockfd,5))==0)
        printf("LISTEN SUCESSFUL\n");

    socklen_t sin_size = sizeof(struct sockaddr_in);
    if((connfd=accept(sockfd,(struct sockaddr *)&clientaddr,&sin_size))>0)
    printf("ACCEPT SUCCESSFUL\n");

    bzero(str, sizeof(str));
    read(connfd,str,sizeof(str));
    printf("Client- %s\n",str);
    tick=time(NULL);

    snprintf(str,sizeof(str),"%s",ctime(&tick));
    write(connfd,str,100);
    Q:close(sockfd);

    return 0;
}