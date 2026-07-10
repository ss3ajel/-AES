
#define _POSIX_C_SOURCE 200112L
#include<unistd.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include<stdio.h>




int main(){
struct addrinfo hints, *res;
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE; 

getaddrinfo(NULL, "8080", &hints, &res);

int server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
bind(server_fd, res->ai_addr, res->ai_addrlen);
listen(server_fd, 10);
freeaddrinfo(res);
}
