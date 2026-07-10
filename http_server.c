#define _POSIX_C_SOURCE 200112L
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(){
    FILE *fp;
    fp = fopen("index.html", "r");
    
   
    char buffer_response[1024] = {0};
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        strcat(buffer_response, line);  
    }
    fclose(fp);
    
    char header[2002];
    sprintf(header, 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "%s", 
        buffer_response
    );
    
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_adress;
    server_adress.sin_family = AF_INET;
    server_adress.sin_addr.s_addr = INADDR_ANY;
    server_adress.sin_port = htons(8001);
    
    bind(server_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));
    listen(server_socket, 5);
    
    printf("Server running on http://localhost:8001\n");
    
    int client_socket;
    while(1){
        client_socket = accept(server_socket, NULL, NULL);
        send(client_socket, header, strlen(header), 0);
        close(client_socket);
    }
    
    return 0;
}