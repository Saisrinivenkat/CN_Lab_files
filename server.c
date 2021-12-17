#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#define MAX 4096
#define PORT 8080
#define SA struct sockaddr

void check(int val , const char* msg){
	if(val < 0){
		printf("\n%s\n",msg);
		exit(1);	
	}
}

void handle_date(int client){
	time_t time1;
	time(&time1);
	char buffer[MAX];
	int n;
	bzero(buffer,MAX);
	
	sprintf(buffer,"Current Date and Time is : %s\n",ctime(&time1));
	printf("%s\n",buffer);
	
	n = send(client,buffer,MAX,0);
	check(n,"Send Error");
	 	
	
}

float extract(char* string){
	char *buffer;
	
	buffer = strtok(string,"\n");
	int i = 0;
	float ans;
	while(buffer != NULL){
//		printf("%s\n",buffer);
		if(i == 4){
			ans = atof(buffer);	
			break;
		}
		buffer = strtok(NULL,"\n");
		i++;	
	}
	free(string);
	return ans;
}

void handle_cgpa(int client){
	int n;
	char buffer[MAX];
	
	float sum = 0;
	printf("\n<--List of Courses and marks from H2-->\n");
	for(int i =0; i < 5;i++){
		bzero(buffer,MAX);
		n = read(client,buffer,MAX);
		char * ext = (char*) malloc(sizeof(char)*MAX);
		strncpy(ext,buffer,strlen(buffer));
		sum += extract(ext);
		printf("%s\n",buffer);
	}
	
	bzero(buffer,sizeof(MAX));
	sprintf(buffer,"Calculating...");	
	n = send(client,buffer,MAX,0);
	check(n,"Send Error");

	printf("Calculating...\nMetaData{\n");
	
	printf("Total Marks: %f",sum);
	float avg = sum/5;
	
//	5 -> five Subject each of 3 credits
	float cgpa = sum * (3*5);	
	
	printf("\n}");
	
	bzero(buffer,sizeof(MAX));
	sprintf(buffer,"\nGrade Point Average : %.2f\nCummalative Grade Point Average: %.2f",avg,cgpa);	
	n = send(client,buffer,MAX,0);
	check(n,"Send Error");

};
void handle_client(int client){
	int n;
	char buffer[MAX];
	sprintf(buffer,"\n<---Select Options From below--->\n\t1.%s\n\t2.%s\nChoose Operation : ","Get Date","Calculate CGPA");
	
	n = send(client,buffer,MAX,0);
	check(n,"Send Error");
	
	bzero(buffer,MAX);
	n = read(client,buffer,MAX);
	printf("%s\n",buffer);
	
	if(buffer[0] == '1'){
		handle_date(client);	
	}else{
		handle_cgpa(client);
	}
}
int main()
{
	int sockfd, connfd, len;
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

	for(;;){
		len = sizeof(cli);
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd < 0)
	    	{
			printf("SERVER ACCEPT FAILED...\n");
			exit(0);
		}
		else
			printf("SERVER ACCEPTED THE CLIENT...\n");
		handle_client(connfd);
	}


	
	close(sockfd);
}
