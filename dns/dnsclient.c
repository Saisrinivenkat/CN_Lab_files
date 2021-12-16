#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>

int main()
{
	char buf[100];
	int k;
	int sock_desc;
	struct sockaddr_in server;
	sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_desc == -1)
	{
		printf("Socket creation failed");
		exit(1);
	}
	printf("Socket successfully created\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(5035);

	k = connect(sock_desc, (struct sockaddr *)&server, sizeof(server));
	if (k == -1)
	{
		printf("Connection to server failed");
		exit(1);
	}

	printf("\nEnter the URL : ");
	fgets(buf, 100, stdin);
	k = send(sock_desc, buf, 100, 0);
	if (k == -1)
	{
		printf("Error in sending");
		exit(1);
	}
	k = recv(sock_desc, buf, 100, 0);
	if (k == -1)
	{
		printf("Error in Receiving");
		exit(1);
	}
	printf("IP Address is : %s", buf);

	close(sock_desc);
	exit(0);
	return 0;
}
