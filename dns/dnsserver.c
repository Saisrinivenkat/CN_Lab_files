#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main()
{
	char buf[100];
	int k;
	socklen_t len;
	int sock_desc, temp_sock_desc;
	struct in_addr address;
	struct hostent *hp;
	struct sockaddr_in server, client;
	sock_desc = socket(AF_INET, SOCK_STREAM, 0);

	if (sock_desc == -1)
	{
		printf("Socket creation failed");
		exit(1);
	}

	printf("\nSocket successfully created\n");
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5035);
	k = bind(sock_desc, (struct sockaddr *)&server, sizeof(server));

	if (k == -1)
	{
		printf("Error in Binding");
		exit(1);
	}

	printf("\nSuccessfully Binded\n");

	k = listen(sock_desc, 20);
	if (k == -1)
	{
		printf("Error in Listening");
		exit(1);
	}
	printf("\nListening Successful\n");
	len = sizeof(client);
	temp_sock_desc = accept(sock_desc, (struct sockaddr *)&client, &len);
	if (temp_sock_desc == -1)
	{
		printf("Error in tempsocket creation");
		exit(1);
	}

	k = recv(temp_sock_desc, buf, 100, 0);
	if (k == -1)
	{
		printf("Error in Receiving");
		exit(1);
	}

	printf("\nURL given by client : %s", buf);
	int bufferlength = strlen(buf);
	buf[bufferlength - 1] = '\0';
	hp = gethostbyname(buf);
	if (hp)
	{
		bcopy(*hp->h_addr_list, (char *)&address, sizeof(address));
		k = send(temp_sock_desc, inet_ntoa(address), 100, 0);
	}
	else
	{
		k = send(temp_sock_desc, buf, 100, 0);
	}

	if (k == -1)
	{
		printf("Error in sending");
		exit(1);
	}

	close(temp_sock_desc);
	exit(0);
	return 0;
}
