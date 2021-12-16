
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
    write(sockfd,"WHAT IS THE TIME",strlen("what is the time"));

    if((n=read(sockfd,buff,sizeof(buff)))<0)
    {
        printf("\nError in Reading");
        exit(0);
    }
    else
    {
        printf("Server- %s\n",buff);
    }
    close(sockfd);
    return 0;
}
