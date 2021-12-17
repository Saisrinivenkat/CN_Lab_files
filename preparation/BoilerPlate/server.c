#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#define SA struct sockaddr_in
#define SAS struct sockaddr
#define BACK_LOG 5
#define MAX 4096

void e_n_e(const char *msg)
{
  printf("\nError: %s", msg);
  exit(1);
}

void check(int t, const char *msg)
{
  if (t < 0)
  {
    e_n_e(msg);
  }
  return;
}

int tcp_client(int PORT)
{
  int server;
  check(server = socket(AF_INET, SOCK_STREAM, 0), "Socket Creation");

  SA myaddr;
  bzero(&myaddr, sizeof(myaddr));
  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(PORT);
  myaddr.sin_addr.s_addr = INADDR_ANY;

  check(bind(server, (SAS *)&myaddr, sizeof(SA)), "Bind Failed");

  check(listen(server, BACK_LOG), "Can't Listen");

  printf("Server Listening On Port:%d\n", PORT);
  return server;
}

int main()
{
  int PORT = 3000;
  int server = tcp_client(PORT);

  SA cliaddr;
  socklen_t len = sizeof(cliaddr);
  bzero(&cliaddr, sizeof(cliaddr));
  int client = accept(server, (SAS *)&cliaddr, &len);

  printf("Client Connected!\nIPADDRESS='%u'\nPORT=%d\n", ntohl(cliaddr.sin_addr.s_addr), ntohs(cliaddr.sin_port));

  return 0;
}
