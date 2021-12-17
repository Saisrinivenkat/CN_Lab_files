#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 256

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

int tcp_client(const char *addr, int port)
{

  int client;
  client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0)
    e("Socket creation error");

  // Optional Bind
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  bzero(&address, addrlen);

  address.sin_family = AF_INET;
  address.sin_port = htons(port);

  check(inet_pton(AF_INET, addr, &address.sin_addr), "Address error");

  check(connect(client, (struct sockaddr *)&address, addrlen), "Connection error");

  return client;
}

void r_and_w(int client)
{
  int n;
  char sbuffer[MAXLINE] = {0}, rbuffer[MAXLINE] = {0};

  while (strncmp(rbuffer, "exit", 4) != 0)
  {
    printf("\nClient : ");
    scanf("%[^\n]s", sbuffer);
    write(client, sbuffer, MAXLINE);
    bzero(sbuffer, MAXLINE);
    bzero(rbuffer, MAXLINE);

    read(client, &rbuffer, MAXLINE);
    printf("Server: %s", rbuffer);
    fflush(stdout);
  }
  // int n;
  // char buffer[255] = {0};

  // do
  // {
  //   bzero(buffer, 255);
  //   printf("\nClient : ");
  //   fgets(buffer, 255, stdin);
  //   send(client, buffer, 255, 0);
  //   bzero(buffer, 255);

  //   recv(client, &buffer, 255, 0);
  //   printf("Server : %s", buffer);
  // } while (strncmp(buffer, "exit", 4));
}

int main()
{
  int client = tcp_client("127.0.0.1", 4020);

  r_and_w(client);
  close(client);
}