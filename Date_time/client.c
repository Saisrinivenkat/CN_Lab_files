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
  address.sin_port = htons(port);

  int opt = 1;
  if (setsockopt(client, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    e("Options error");

  // if (bind(client, (struct sockaddr *)&address, addrlen))
  //   e("Bind Error");

  // if (inet_pton(AF_INET, addr, &address.sin_addr) <= 0)
  //   e("Address error");

  address.sin_port = htons(port);

  if (connect(client, (struct sockaddr *)&address, addrlen))
    e("Connection error");

  return client;
}

void r_and_w(int client)
{
  int n;
  char buffer[255] = "Saisrini";

  printf("\nRequest Sent");

  send(client, buffer, 255, 0);
  // recv(client, &buffer, 255, 0);
  printf("\nFrom Server : %s", buffer);
}

int main()
{
  int client = tcp_client("127.0.0.1", 3000);
  printf("Connected");
  r_and_w(client);
  close(client);
}