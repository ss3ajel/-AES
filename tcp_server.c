#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdio.h>
int main(){
    int fd;
    fd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server_adress;
    server_adress.sin_family=AF_INET;
    server_adress.sin_port=htons(9002);
    server_adress.sin_addr.s_addr=inet_addr("127.0.0.1");
    int connection_status=connect(fd,(struct sockaddr*)&server_adress,sizeof(server_adress));
    if (connection_status==-1){
        printf("failed to connect");
    }
    char message[256];
    recv(fd, message,sizeof(message),0);
    printf("%s",message);
    close(fd);
    return 0;



        
    
}