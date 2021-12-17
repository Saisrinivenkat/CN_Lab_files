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
  int server = socket(AF_INET, SOCK_DGRAM, 0);
  check(server, "Socket Error");

  SA cliaddr, servaddr;
  bzero(&cliaddr, sizeof(SA));

  cliaddr.sin_addr.s_addr = INADDR_ANY;
  cliaddr.sin_port = htons(3908);
  cliaddr.sin_family = AF_INET;

  check(bind(server, (SAS *)&cliaddr, sizeof(SA)), "Bind Failed");

  char buffer[1024];
  socklen_t len = sizeof(SA);
  int n;
  check(n = recvfrom(server, buffer, 1024, MSG_WAITALL, (SAS *)&servaddr, &len), "Read Error");
  buffer[n] = '\0';

  printf("Client asks '%s'\n", buffer);
  bzero(buffer, 1024);

  time_t t;
  time(&t);
  // printf("%s\n", ctime(&t));
  sprintf(buffer, "Server says '%s'", ctime(&t));

  check(n = sendto(server, buffer, 1024, MSG_SEND, (SAS *)&servaddr, sizeof(SA)), "Write Error");
}