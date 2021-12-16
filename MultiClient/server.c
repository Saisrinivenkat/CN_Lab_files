#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define MAX 4

void e(const char *msg)
{
  perror(msg);
  exit(1);
}

int tcp_server(int PORT)
{

  int server;
  server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0)
    e("Socket Creation error");

  int opt = TRUE;
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(opt)) < 0)
    e("Option Set Unsuccessful");

  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  bzero(&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  if (bind(server, (struct sockaddr *)&address, addrlen) < 0)
    e("Bind Unsuccessfull");

  if (listen(server, MAX) < 0)
    e("Can't Listen");

  return server;
}

void r_and_w(int client)
{
  int n;
  char buffer[255] = {0};

  while ((n = recv(client, &buffer, 255, 0)) > 0)
  {
    printf("\nClient : %s", buffer);
    bzero(buffer, 255);

    printf("Server : ");
    fgets(buffer, 255, stdin);
    send(client, buffer, 255, 0);
  }
}

int main()
{

  int server = tcp_server(2020);

  while (1)
  {
    struct sockaddr_in cliaddress;
    socklen_t addrlen = sizeof(cliaddress);
    bzero(&cliaddress, addrlen);
    printf("Server Listening\n");

    int client = accept(server, (struct sockaddr *)&cliaddress, &addrlen);
    if (client < 0)
      e("Can't Accept Connections");

    printf("\nClient Connected from IP: %s and PORT: %d\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));

    int pid = fork();

    if (pid)
    {
      r_and_w(client);
    }

    close(client);
  }
  return 0;
}