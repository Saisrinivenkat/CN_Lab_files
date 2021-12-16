#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "stdlib.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXLINE 4096
#define SA struct sockaddr_in

void check(int d, const char *s)
{
  if (d >= 0)
  {
    return;
  }
  printf("\n%s\n", s);
  exit(0);
}

int httpClient(char *ip, int PORT)
{
  int client;

  check(client = socket(AF_INET, SOCK_STREAM, 0), "Socket Creation Error");

  SA servaddr;
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_port = htons(PORT);
  servaddr.sin_family = AF_INET;

  if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0)
  {
    printf("\nError in pton for addr: %s\n", ip);
    exit(0);
  }

  check(connect(client, (struct sockaddr *)&servaddr, sizeof(servaddr)), "Server Connection error");

  fprintf(stdout, "Connected to Server!\n");
  return client;
}

void handleResponse(int client)
{
  char buffer[MAXLINE];
  bzero(buffer, MAXLINE);

  sprintf(buffer, "GET / HTTP/1.1\r\n\r\n");

  if (write(client, buffer, sizeof(buffer)) != sizeof(buffer))
  {
    printf("Writing Error");
    exit(0);
  }

  int size;
  while ((size = read(client, buffer, sizeof(buffer))) > 0)
  {
    fprintf(stdout, "%s", buffer);
  }

  check(size, "Read error");
}

int main()
{
  char ip[20];
  printf("<IP Address> : ");
  scanf("%s", ip);

  int client = httpClient(ip, 80);

  handleResponse(client);
  close(client);
  return 0;
}