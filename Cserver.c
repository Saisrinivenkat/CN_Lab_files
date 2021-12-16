//
// Created by saisrini on 08/10/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char * msg){
    perror(msg);
    exit(1);
}

int tcp_server(int PORT){
    //Create Server
    int servseocket;

    servseocket = socket(AF_INET,SOCK_STREAM,0);
    if(servseocket < 0 ){
        error("Socket Creation");
    }

    //Set Options
    int opt = 1;
    if(setsockopt(servseocket,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt)) < 0 ){
        error("Options");
    }

    struct sockaddr_in address;

    memset(&address,0,sizeof(address));

    //Set Address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(servseocket,(struct sockaddr *)&address, sizeof(address)) < 0){
        error("Bind");
    }

    if(listen(servseocket,3)  < 0){
        error("Listen");
    }
    fprintf(stdout,"Server Started ! Waiting for Client");
    return servseocket;
}


int main(){
    int serv = tcp_server(5000);

    //Connecting Client
    int client;
    struct sockaddr_in cliAddress;
    socklen_t size_add = sizeof(cliAddress);

    client = accept(serv,(struct sockaddr*)&cliAddress, &size_add );
    if(client < 0){
        error("Accept Failed");
    }

    char buffer[255] = {0};

    recv(client,buffer,255,0);
    printf("Client : ");
    printf("%s", buffer);
    printf("\n");

    memset(&buffer,0, sizeof(buffer));

    printf("Server : ");
    fgets(buffer,1024,stdin);
    send(client,buffer,255,0);
    printf("Response Sent");

    close(client);
    close(serv);
    return 0;
}