void r_and_w(int client)
{
  char buffer[255] = {0};

  recv(client, &buffer, 255, 0);
  printf("\nClient : %s\n", buffer);
  bzero(buffer, 255);

  printf("Server : ");
  fgets(buffer, 255, stdin);
}