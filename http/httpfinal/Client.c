#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#define MYPORT 6780
#define MAX 1024

int main()
{

	int sockfd, status_code = 404;
	char *status_message = (char *)malloc(sizeof(char) * MAX);
	char *file_path = (char *)malloc(sizeof(char) * MAX);
	char *GET_string = (char *)malloc(sizeof(char) * MAX);
	char *Response_string = (char *)malloc(sizeof(char) * MAX);

	struct sockaddr_in Client_addr;
	Client_addr.sin_family = AF_INET;
	Client_addr.sin_port = htons(MYPORT);
	Client_addr.sin_addr.s_addr = INADDR_ANY;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("\nSOCKET not created:");
		exit(1);
	}
	printf("\nSocket created");

	if (connect(sockfd, (struct sockaddr *)&Client_addr, sizeof(Client_addr)) == -1)
	{
		perror("\nNot connected:");
		exit(1);
	}
	printf("\nConnected");

	printf("\nEnter URL:");
	scanf("%[^\n]s", file_path);

	snprintf(GET_string, MAX, "GET /%s HTTP/1.1\r\nHost:www.MyWebsite.com\r\nContent-Type:text/html\r\n", file_path);
	send(sockfd, GET_string, strlen(GET_string), 0);
	printf("\nGET Request sent\n");

	read(sockfd, Response_string, MAX);
	printf("\n%s", Response_string);

	close(sockfd);
}
