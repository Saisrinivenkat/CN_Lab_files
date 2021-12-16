#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define MAX 4

void e(char *msg)
{
  perror(msg);
  exit(1);
}

int smtp_server(int PORT)
{

  int server;
  server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0)
    e("Socket Creation error");

  int opt = TRUE;
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
int validate(char *email)
{
  char *ptr;
  for (ptr = email; *ptr; ptr++)
  {
    if (*ptr == '@' && ptr != email)
    {
      ptr++;
      break;
    }
    else if (*ptr == ' ')
    {
      return 1;
    }
  }
  if (!(*ptr))
  {
    return 1;
  }
  char *ptr1;
  int count = 0;
  for (ptr1 = ptr; *ptr1; ptr1++, count++)
  {
    if (*ptr1 == '.' && ptr1 != ptr)
    {
      ptr1++;
      break;
    }
    else if (*ptr1 == ' ')
    {
      return 1;
    }
  }
  if (count < 1)
  {
    return 1;
  }
  count = 0;
  while (*ptr1)
  {
    count++;
    ptr1++;
  }
  if (count < 2 || count > 3)
  {
    return 1;
  }
  return 0;
}

char *extract(char *mail)
{
  char delim[] = "\n";
  char *tmail = malloc(sizeof(char) * 50);
  char *ptr = strtok(mail, delim);
  int i = 1;
  while (ptr != NULL)
  {
    if (i == 2)
    {
      ptr += 4;
      strcpy(tmail, ptr);
      break;
    }
    i++;
    ptr = strtok(NULL, delim);
  }
  return tmail;
}

void r_and_w(int client)
{

  int n;
  char sentBuffer[255];
  char recBuffer[255];
  char timeString[50];
  bzero(sentBuffer, 255);
  bzero(recBuffer, 255);

  time_t timec;
  struct tm *lc_time;

  timec = time(NULL);
  lc_time = localtime(&timec);
  sprintf(timeString, "\nDate Sent: %s", asctime(lc_time));

  recv(client, recBuffer, 255, 0);
  char mail[310];
  sprintf(mail, "%s%s", recBuffer, timeString);
  char *tmail = extract(recBuffer);

  int check = validate(tmail);
  if (check == 1)
  {
    sprintf(sentBuffer, "\n%s", "User Mail Invalid!");
    printf("\nMail sent Unsuccessful!");
  }
  else
  {
    printf("%s", mail);
    printf("\nMail Forwarded Successfully!");

    sprintf(sentBuffer, "\n%s", "Mail Sent Successful!");
  }
  send(client, sentBuffer, 255, 0);
}

int main()
{
  int port = 5001;
  int server = smtp_server(port);

  struct sockaddr_in cliaddress;
  socklen_t addrlen = sizeof(cliaddress);
  bzero(&cliaddress, addrlen);
  printf("Server Listening\n");

  int client = accept(server, (struct sockaddr *)&cliaddress, &addrlen);
  if (client < 0)
    e("Can't Accept Connections");

  printf("\nClient Connected from IP: %s and PORT: %d\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));

  r_and_w(client);

  close(client);
  close(server);
  return 0;
}
