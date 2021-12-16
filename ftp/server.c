#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 4

void e(char *msg)
{
  perror(msg);
  exit(1);
}
int ftp_server(int PORT)
{

  int server;
  server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0)
    e("Socket Creation error");

  int opt = 1;
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)sizeof(opt)) < 0)
    e("Option Set Unsuccessful");

  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  bzero(&address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(PORT);

  if (bind(server, (struct sockaddr *)&address, addrlen) < 0)
    e("Bind Unsuccessfull");

  if (listen(server, MAX) < 0)
    e("Can't Listen");

  return server;
}

void sendFile(int client)
{
  char recvBuffer[255], sentBuffer[255];

  recv(client, recvBuffer, 255, 0);
  printf("\nClient: Open File -> %s", recvBuffer);
  FILE *fp;
  fp = fopen(recvBuffer, "r");
  bzero(recvBuffer, 255);

  if (fp == NULL)
  {
    sprintf(recvBuffer, "\n%s", "404 File Not Found");
    printf("\n%s", recvBuffer);
    send(client, recvBuffer, 255, 0);
  }
  else
  {
    sprintf(recvBuffer, "\n%s", "File Found\nSending...");
    send(client, recvBuffer, 255, 0);
    while (fgets(recvBuffer, 254, fp))
    {
      recvBuffer[strcspn(recvBuffer, "\n")] = 0;
      send(client, recvBuffer, 255, 0);
      bzero(recvBuffer, 255);
    }
    sprintf(recvBuffer, "end");
    send(client, recvBuffer, 255, 0);
  }

  fclose(fp);
}

int main()
{
  int port = 5001;
  int server = ftp_server(port);

  struct sockaddr_in cliaddress;
  socklen_t addrlen = sizeof(cliaddress);
  bzero(&cliaddress, addrlen);
  printf("Server Listening\n");

  int client = accept(server, (struct sockaddr *)&cliaddress, &addrlen);
  if (client < 0)
    e("Can't Accept Connections");

  printf("\nClient Connected from IP: %s and PORT: %d\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));

  sendFile(client);

  close(client);
  close(server);
  return 0;
}