#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

void e(char *msg)
{
  perror(msg);
  exit(1);
}

int tcp_client(const char *addr, int port)
{

  int client;
  client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0)
    e("Socket creation error");

  //Optional Bind
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  bzero(&address, addrlen);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(18523);

  int opt = 1;
  if (setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    e("Options error");

  if (bind(client, (struct sockaddr *)&address, addrlen))
    e("Bind Error");

  if (inet_pton(AF_INET, addr, &address.sin_addr) <= 0)
    e("Address error");

  address.sin_port = htons(port);

  if (connect(client, (struct sockaddr *)&address, addrlen))
    e("Connection error");

  return client;
}

void r_and_w(int client)
{
  int n, choice, num1, num2, result;
  char buffer[256] = {0};

  read(client, buffer, 256);
  printf("Server- %s\n", buffer);
  scanf("%d", &choice);

  write(client, &choice, sizeof(int));
  bzero(buffer, 256);

  read(client, buffer, 256);
  printf("Server- %s\n", buffer);
  scanf("%d", &num1);

  write(client, &num1, sizeof(int));
  bzero(buffer, 256);

  n = read(client, buffer, 256);
  printf("Server- %s\n", buffer);
  scanf("%d", &num2);
  write(client, &num2, sizeof(int));

  read(client, &result, sizeof(result));
  printf("Server- The answer is %d\n", result);
}

int main()
{
  int client = tcp_client("127.0.0.1", 2020);
  printf("Connected");
  r_and_w(client);
  close(client);
}