
#define _POSIX_C_SOURCE 200112L
#include<unistd.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include<stdio.h>
int main(){
    int status;
    int sockfd;
    struct addrinfo hints ,*res,*p;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = AI_PASSIVE; 
    status = getaddrinfo("www.example.net", "3490", &hints, &res);
    if ((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    exit(1);
}
for(p=res;p!=NULL;p=p->ai_next){
    /*void *addr;
    char *ipver;
     char ipstr[INET6_ADDRSTRLEN];
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;
    if(p->ai_family==AF_INET){
        ipv4=(struct sockaddr_in*)p->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver="IPV4";

    }else{
        ipv6=(struct sockaddr_in6*)p->ai_addr;
        addr=&(ipv6->sin6_addr);
        ipver="IPV6";
    }
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);*/
        sockfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(sockfd==-1) connect;
        if (connect(sockfd,p->ai_addr, p->ai_addrlen)) break;
        close(sockfd);
        

        
}




freeaddrinfo(res);

}