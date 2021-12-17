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
  printf("\nError: %s\n", msg);
  exit(1);
}

void check(int check, const char *msg)
{
  if (check < 0)
  {
    e_n_e(msg);
  }
}

int connectServer(int PORT, const char *ip)
{
  int client = socket(AF_INET, SOCK_STREAM, 0);
  check(client, "\nCreation Error");

  SA servaddr;
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = inet_addr(ip);

  check(connect(client, (SAS *)&servaddr, (socklen_t)sizeof(servaddr)), "Connection Problem");

  printf("\nConnected to the Server!\n");
  return client;
}

void handle_server(int client)
{
  int n;
  char buffer[MAX];

  char *req = "values.txt\0";
  sprintf(buffer, "%s", req);

  n = send(client, buffer, MAX, 0);
  check(n, "Write error");

  while ((n = read(client, buffer, MAX)) > 0)
  {
    buffer[n] = '\0';
    printf("%s", buffer);
    if (buffer[strlen(buffer) - 1] == '\n')
    {
      break;
    }
  }
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    e_n_e("./client <PORT>\nformat error");
  }
  int PORT = atoi(argv[1]);

  printf("%d", PORT);

  int client = connectServer(PORT, "127.0.0.1");

  handle_server(client);

  close(client);
  return 0;
}