#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main(){
    char message[256]="hello ma nigaas";
    int fd;
    fd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_adress;
    server_adress.sin_family=AF_INET;
    server_adress.sin_port=htons(9002);
    server_adress.sin_addr.s_addr=inet_addr("127.0.0.1");
    int binded=bind(fd,(struct sockaddr*) &server_adress,sizeof(server_adress));
    listen(fd,5);
    int client_socket;
    client_socket=accept(fd,NULL,NULL);
    send(client_socket,&message,sizeof(message),0);
    close(fd);
    return 0;
}