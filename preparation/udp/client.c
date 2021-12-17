#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SA struct sockaddr_in
#define SAS struct sockaddr

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

int main()
{
  int client = socket(AF_INET, SOCK_DGRAM, 0);
  check(client, "Socket Failed");
  char buffer[1024];
  bzero(buffer, 1024);

  SA servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(3908);
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int n = sendto(client, buffer, 1024, MSG_SEND, (SAS *)&servaddr, sizeof(SA));
  check(n, "Write Error");
  printf("%s\n", buffer);

  bzero(buffer, 1024);
  socklen_t len = sizeof(SA);
  n = recvfrom(client, buffer, 1024, MSG_WAITALL, (SAS *)&servaddr, &len);
  check(n, "Write Error");
  printf("%s\n", buffer);
  return 0;
}