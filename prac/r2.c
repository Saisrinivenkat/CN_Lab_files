#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *my_itoa(int num, char *str)
{
  if (str == NULL)
  {
    return NULL;
  }
  sprintf(str, "%d", num);
  return str;
}

int main()
{
  int n;
  scanf("%d", &n);
  int m;
  scanf("%d", &m);

  char p[1];
  my_itoa(m, p);

  char a[10];

  my_itoa(n, a);

  char b[10];

  int j = 0;
  for (size_t i = 0; a[i] != '\0'; i++)
  {
    if (a[i] != p[0])
    {
      b[j] = a[i];
      j++;
    }
  }

  b[j] = '\0';

  int f = atoi(b);

  printf("%d\n", (f % m));
  if (f % m == 0)
  {
    printf("Divisible");
  }
  else
  {
    printf("Not Divisible");
  }
  return 0;
}