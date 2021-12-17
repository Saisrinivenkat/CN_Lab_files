#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#define MAX 4096
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) 
    {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n');
		write(sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) 
        {
			printf("CLIENT EXIT...\n");
			break;
		}
	}
}

void check(int val , const char* msg){
	if(val < 0){
		printf("\n%s\n",msg);
		exit(1);	
	}
}

void handle_cgpa(int client){
	int n;
	char buffer[MAX];

	char courses[10][100] = {"WEB TECH","COMPUTER NETWORKS","COMPILER ENG","PATTERN RCNG.","PSCHOLOGY"};
	int marks[] = {85,96,87,46,99};
	
	for(int i = 0; i < 5;i++){
		bzero(buffer,MAX);
		sprintf(buffer,"{\n  Subject Name:\n\t%s\n  Mark:\n\t%d\n}",courses[i],marks[i]);
		n = send(client,buffer,MAX,0);
		
		check(n,"Send Error");	
	}
	bzero(buffer,MAX);
	n = read(client,buffer,MAX);
	check(n,"Read Error");

	printf("\n%s",buffer);

	bzero(buffer,MAX);
	n = read(client,buffer,MAX);
	check(n,"Read Error");

	printf("\nServer says ->\n%s\n",buffer);
}

void handle_func(int client){
	int n;
	char buffer[MAX];

	bzero(buffer,MAX);
	n = read(client,buffer,MAX);
	check(n,"Read Error");
	
	
	printf("\nServer says ->\n%s",buffer);
	fflush(stdout);
	bzero(buffer,MAX);
	scanf("%s",buffer);
	
	n = send(client,buffer,MAX,0);
	check(n,"Send Error");

	if(buffer[0] == '1'){
		bzero(buffer,MAX);
		n = read(client,buffer,MAX);
		check(n,"Read Error");
		
		printf("\nServer says ->\n\t%s",buffer);
		fflush(stdout);
		bzero(buffer,MAX);
	}else{
		handle_cgpa(client);
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) 
    {
		printf("CONNECTION WITH THE SERVER FAILED...\n");
		exit(0);
	}
	else
		printf("CONNECTED TO THE SERVER..\n");


	handle_func(sockfd);
	close(sockfd);
}
