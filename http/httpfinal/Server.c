#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>

#define MYPORT 6780
#define MAX 2014
#define BACKLOG 5

char *handle_GET(char *);
char *get_status_message();
int status_code = 0;

int main()
{

	char *buff = (char *)malloc(sizeof(char) * MAX);
	char *data = (char *)malloc(sizeof(char) * MAX);
	char *Response_string = (char *)malloc(sizeof(char) * MAX);
	time_t t;
	time(&t);
	int ServerSockfd, ClientSockfd;

	socklen_t sin_size;

	struct sockaddr_in Server_addr, Client_addr;

	Server_addr.sin_family = AF_INET;
	Server_addr.sin_addr.s_addr = INADDR_ANY;
	Server_addr.sin_port = htons(MYPORT);

	if ((ServerSockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket not created:");
		exit(1);
	}

	printf("\nSocket created");

	if (bind(ServerSockfd, (const struct sockaddr *)&Server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("\nNot Bound");
		exit(1);
	}
	else
		printf("\nBind Successful");

	if (listen(ServerSockfd, BACKLOG) == -1)
	{
		perror("\nCan't listen");
		exit(1);
	}
	else
		printf("\nListening.....");

	if ((ClientSockfd = accept(ServerSockfd, (struct sockaddr *)&Client_addr, &sin_size)) == -1)
	{
		perror("\nCan't accept");
		exit(1);
	}

	printf("\nClient request accepted");

	if (read(ClientSockfd, buff, MAX) == -1)
	{
		perror("\nReader failed:");
		exit(1);
	}

	if (strncmp(buff, "GET", 3) == 0)
	{

		printf("\n\n%s", buff);
		data = handle_GET(buff);

		snprintf(Response_string, MAX, "HTTP/1.1 %s\r\nServer:LocalServer\r\nDate:%sContent-type:text\\html\r\n", get_status_message(), ctime(&t));

		if (status_code == 200)
		{
			strcat(Response_string, data);
		}

		send(ClientSockfd, Response_string, strlen(Response_string), 0);
		printf("\nResponse sent");
	}

	close(ServerSockfd);
}

char *handle_GET(char *buff)
{
	char *file_path;
	char *data;

	file_path = (char *)malloc(sizeof(char) * MAX);
	data = (char *)malloc(sizeof(char) * MAX);

	int i = 3;

	while (i < sizeof(buff) && buff[i] != '/')
	{
		i++;
	}

	i++;

	while (buff[i] != '.')
	{
		strncat(file_path, &buff[i], 1);
		i++;
	}

	while (buff[i] != ' ')
	{
		file_path = strncat(file_path, &buff[i], 1);
		i++;
	}

	FILE *fp = fopen(file_path, "r");

	if (fp == NULL)
	{
		status_code = 404; //incorrect file path(client error)
		perror("\nFile not opened");
		return NULL;
	}

	while (fgets(buff, MAX, fp))
	{
		strcat(data, buff);
	}

	fclose(fp);
	if (data == NULL)
	{ //if the server can't able to extract data(Server error)
		status_code = 500;
		return NULL;
	}
	else
	{
		status_code = 200; //success
		return data;
	}
}

char *get_status_message()
{

	char *status_message = (char *)malloc(sizeof(char) * MAX);

	switch (status_code)
	{
	case 200:
		status_message = "200 OK";
		break;
	case 404:
		status_message = "404 File not found";
		break;
	case 500:
		status_message = "500 Server Error";
		break;
	}

	return status_message;
}

//Users/saisrini/Downloads/files/sample1.txt