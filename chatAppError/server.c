#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>

#define TRUE 1
#define FALSE 0

#define MAX 4
#define MAXLINE 255

void e(const char *msg)
{
  perror(msg);
  exit(1);
}

void check(int x, const char *msg)
{
  if (x >= 0)
  {
    return;
  }
  e(msg);
}

int tcp_server(int PORT)
{
  int server;
  check(server = socket(AF_INET, SOCK_STREAM, 0), "Socket Creation error");

  int opt = TRUE;
  check(setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(opt)), "Option Set Unsuccessful");

  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  bzero(&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  check(bind(server, (struct sockaddr *)&address, addrlen), "Bind Unsuccessfull");

  check(listen(server, MAX), "Can't Listen");

  printf("\nServer Listening on Port: %d\n", PORT);
  return server;
}

void r_and_w(int client)
{

  int n;
  char sbuffer[MAXLINE] = {0}, rbuffer[MAXLINE] = {0};
  while (strncmp(rbuffer, "exit", 4) != 0)
  {
    bzero(rbuffer, MAXLINE);
    n = read(client, &rbuffer, MAXLINE);
    if (n < 0)
    {
      e("Read Error;");
    }
    printf("Client: %s\n", rbuffer);
    bzero(sbuffer, MAXLINE);
    printf("Server : ");
    fflush(stdout);
    fgets(sbuffer, MAXLINE, stdin);
    write(client, sbuffer, MAXLINE);
  }
  // int n;
  // char buffer[255] = {0};
  // while ((n = recv(client, &buffer, 255, 0)) > 0)
  // {
  //   printf("\nClient : %s", buffer);
  //   if (strcmp(buffer, "exit") == 0)
  //     return;
  //   bzero(buffer, 255);

  //   printf("Server : ");
  //   fgets(buffer, 255, stdin);
  //   send(client, buffer, 255, 0);
  //   bzero(buffer, 255);
  // }
  // return 1;
  // gcc server.c && ./a.out
}

int main()
{
  int port = 4020;
  int server = tcp_server(port);

  struct sockaddr_in cliaddress;
  socklen_t addrlen = sizeof(cliaddress);
  bzero(&cliaddress, addrlen);

  while (1)
  {
    int client = accept(server, (struct sockaddr *)&cliaddress, &addrlen);
    check(client, "Can't Accept Connections");

    printf("\nClient Connected from IP: %s and PORT: %d\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));

    r_and_w(client);
    close(client);
  }
  close(server);
  return 0;
}