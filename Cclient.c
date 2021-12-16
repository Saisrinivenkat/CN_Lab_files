//
// Created by saisrini on 08/10/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char * msg){
    perror(msg);
    exit(1);
}

int tcp_client(const char* address,int port){
    int socket1;

    socket1 = socket(AF_INET,SOCK_STREAM,0);
    if (socket1 <0)
        error("Socket Creation");

    struct sockaddr_in servaddress;
    socklen_t add_len = sizeof(servaddress);

    servaddress.sin_family = AF_INET;
    servaddress.sin_port = htons(port);

    //Convert to network format
    if( (inet_pton(socket1,"127.0.0.1",(struct sockaddr*)&servaddress.sin_addr)) < 0){
        error("Address Format");
    }

    if( connect(socket1,(struct sockaddr *)&servaddress, add_len) < 0 ){
        error("Connection");
    }
    printf("Connected to the server!\n");
    return socket1;
}

int main(){

    int client = tcp_client("127.0.0.1",5000);

    char buffer[255] = {0};
    printf("Client : ");
    fgets(buffer,255,stdin);
    send(client,buffer,255,0);

    memset(&buffer,0,sizeof(buffer));

    printf("Server: ");
    recv(client,buffer,255,0);
    printf("%s",buffer);

    close(client);
    return 0;
}
