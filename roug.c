#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  for (ptr1 = ptr; *ptr1; ptr1++)
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
  int count = 0;
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

int main()
{
  char tmail[] = "From: saisrinivenkat@outlook.com\nTo: saisrini@gmail.c\nBody: {\n\tGood Morning! Hey You how are ya?    }";
  char *rslt = extract(tmail);
  printf("%s", rslt);

  int d = validate(rslt);
  printf("\n%d", d);
  return 0;
}