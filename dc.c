//
// Created by saisrini on 08/10/21.
//

#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <unistd.h>
#include<string.h>
#include<strings.h>
#include <arpa/inet.h>

int main()
{
    int b,sockfd,con,n,len;
    int choice;
    char buff[256];
    int op1,op2,result;
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))>0)
        printf("SOCKET CREATED SUCCESSFULLY\n");

    struct sockaddr_in servaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=8080;

    socklen_t sin_size = sizeof(struct sockaddr_in);

    if((con=connect(sockfd,(struct sockaddr *) &servaddr, sin_size))==0)
        printf("CONNECT SUCCESSFUL\n");

    S:bzero(buff,256);
    n=read(sockfd,buff,255);
    printf("Server- %s\n",buff);
    scanf("%d",&choice);

    if(choice==5)
        goto Q;
    write(sockfd,&choice,sizeof(int));
    bzero(buff,256);

    n=read(sockfd,buff,255);
    printf("Server- %s\n",buff);
    scanf("%d",&op1);

    write(sockfd,&op1,sizeof(int));
    bzero(buff,256);

    n=read(sockfd,buff,255);
    printf("Server- %s\n",buff);
    scanf("%d",&op2);
    write(sockfd,&op2,sizeof(int));

    read(sockfd,&result,sizeof(result));
    printf("Server- The answer is %d\n",result);

    if(choice!=5)
        goto S;

    Q:printf("You have selected Exit..\n");
    close(sockfd);
    return 0;
}