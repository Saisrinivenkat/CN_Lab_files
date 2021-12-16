#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "stdlib.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAXLINE 256
#define SA struct sockaddr_in

void e_n_e(char *s)
{
  printf("%s", s);
  exit(0);
}

int httpServer(int PORT)
{

  int server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0)
  {
    e_n_e("Error in creating Socket!");
  }

  int yes = 1;
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, (socklen_t)sizeof(yes)) < 0)
  {
    e_n_e("Can't Resolve Address!");
  }

  SA servaddr;
  socklen_t len = sizeof(servaddr);
  bzero(&servaddr, sizeof(servaddr));

  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);
  servaddr.sin_family = AF_INET;

  if (bind(server, (struct sockaddr *)&servaddr, len) < 0)
  {
    e_n_e("Error in Binding!");
  }

  if (listen(server, 5) < 0)
  {
    e_n_e("Error in Listening!");
  }

  printf("\nServer Started On port: %d\n", PORT);
  fflush(stdout);
  return server;
}

void r_n_w(int client, int server)
{
  char buffer[MAXLINE];
  bzero(&buffer, MAXLINE);

  int size = 0;
  while ((size = read(client, buffer, MAXLINE)) > 0)
  {
    printf("%s\n", buffer);
    bzero(buffer, MAXLINE);
  }

  snprintf(buffer, MAXLINE, "%s", "HTTP/1.0 200 OK\r\n\r\nMy Server!!!");

  write(client, buffer, strlen(buffer));
}

int main()
{
  int PORT = 3000;
  int server = httpServer(3000);

  // while(true){
  int client;
  SA cliaddr;
  socklen_t len;
  bzero(&cliaddr, sizeof(cliaddr));
  if ((client = accept(server, (struct sockaddr *)&cliaddr, &len)) < 0)
  {
    e_n_e("Can't Accept Client");
  }

  printf("Client Connected from IP:%s and PORT:%d\n\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
  // fflush(stdout);

  r_n_w(client, server);

  close(client);
  // }

  return 0;
}