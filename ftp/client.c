#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

void e(char *msg)
{
  perror(msg);
  exit(1);
}
int connectToServer(char *ip, int port)
{

  int cSoc = socket(AF_INET, SOCK_STREAM, 0);
  if (cSoc < 0)
  {
    e("\nError Creating Socket");
  }

  struct sockaddr_in servAddr;
  socklen_t len = sizeof(servAddr);
  bzero(&servAddr, len);

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(port);
  servAddr.sin_addr.s_addr = inet_addr(ip);

  if (connect(cSoc, (struct sockaddr *)&servAddr, len) < 0)
  {
    e("\nError Connecting to Server");
  }

  return cSoc;
}

void getFile(int sock)
{
  char fileName[] = "mail.txt";
  send(sock, fileName, 25, 0);
  char recBuffer[255];
  int check = recv(sock, recBuffer, 255, 0);
  printf("%s", recBuffer);

  if (check != 0)
  {
    bzero(recBuffer, 255);
    recv(sock, recBuffer, 255, 0);
    while (strcmp(recBuffer, "end") != 0)
    {
      printf("\n\t%s", recBuffer);
      bzero(recBuffer, 255);
      recv(sock, recBuffer, 255, 0);
    }
    printf("\n");
    for (size_t i = 0; i < 40; i++)
    {
      printf("-");
    }
  }
}

int main()
{
  int sock = connectToServer("127.0.0.1", 5001);

  if (sock < 0)
    e("\nError unknown");
  else
  {
    printf("\nConnected to Server Successfully");
  }

  getFile(sock);

  close(sock);
  return 0;
}