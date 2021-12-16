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

void sendMail(int sock)
{
  char usrMail[] = "saisrinivenkat@outlook.com";

  char fmail[50];
  sprintf(fmail, "From: %s", usrMail);

  char msg[] = "Good Morning! Hey You how are ya?";

  char body[100];
  sprintf(body, "Body: {\n\t%s\n    }", msg);

  char receiver[] = "annauniv@gmail.com";
  char tmail[50];
  sprintf(tmail, "To: %s", receiver);

  char mailChunk[200];
  sprintf(mailChunk, "\n%s\n%s\n%s", fmail, tmail, body);

  // printf("%s", mailChunk);
  send(sock, mailChunk, 200, 0);

  char recBuf[255];
  recv(sock, recBuf, 255, 0);
  printf("%s", recBuf);
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

  sendMail(sock);

  close(sock);
  return 0;
}