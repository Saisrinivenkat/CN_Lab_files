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

    socklen_t  sin_size = sizeof(struct sockaddr_in);
    if((connfd=accept(sockfd,(struct sockaddr *)&clientaddr,&sin_size))>0)
        printf("ACCEPT SUCCESSFUL\n");

    S:n=write(connfd,"Enter required operation:\n1.Addition\n2.Subtraction\n3.Division\n4.Multiplication\n5.Exit",strlen("Enter required operation:\n1.Addition\n2.Subtraction\n3.Division\n4.Multiplication\n5.Exit"));
    read(connfd,&choice,sizeof(int));
    if(choice==5)
        goto Q;

    printf("Client- choice is: %d\n",choice);
    n=write(connfd,"Enter number 1: ",strlen("Enter number 1: "));

    read(connfd,&op1,sizeof(op1));
    printf("Client- Number 1 is %d\n",op1);
    n=write(connfd,"Enter number 2: ",strlen("Enter number 2: "));

    read(connfd,&op2,sizeof(op2));
    printf("Client- Number 2 is %d\n",op2);

    switch(choice)
    {
        case 1:
            result=op1+op2;
            printf("Result is %d + %d = %d\n",op1, op2, result);
            break;
        case 2:
            result=op1-op2;
            printf("Result is %d - %d = %d\n",op1, op2, result);
            break;
        case 3:
            result=op1/op2;
            printf("Result is %d / %d = %d\n",op1, op2, result);
            break;
        case 4:
            result=op1*op2;
            printf("Result is %d * %d = %d\n",op1, op2, result);
            break;
        case 5:
            goto Q;
            break;
    }
    write(connfd,&result,sizeof(result));

    if(choice!=5)
        goto S;
    Q:close(sockfd);
    return 0;
}