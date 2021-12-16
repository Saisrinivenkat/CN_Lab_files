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

  // Optional Bind
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  bzero(&address, addrlen);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(14523);

  if (bind(client, (struct sockaddr *)&address, addrlen))
    e("Bind Error");

  if (inet_pton(AF_INET, addr, &address.sin_addr) <= 0)
    e("Address error");

  address.sin_port = htons(port);

  if (connect(client, (struct sockaddr *)&address, addrlen))
    e("Connection error");

  return client;
}

int r_and_w(int client)
{
  int n;
  char buffer[255] = {0};

  do
  {
    printf("\nClient : ");
    fgets(buffer, 255, stdin);
    send(client, buffer, 255, 0);
    bzero(buffer, 255);

    recv(client, &buffer, 255, 0);
    printf("Server : %s", buffer);
  } while (strncmp(buffer, "bye", 3));
  return 1;
}

int main()
{
  int client = tcp_client("127.0.0.1", 2020);

  r_and_w(client);
  close(client);
}