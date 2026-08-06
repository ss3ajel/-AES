#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <poll.h>
#include <netinet/tcp.h>
struct bgp_header {
    uint8_t marker[16];
    uint16_t length;
    uint8_t type;
} __attribute__((packed));
unsigned short checksum(unsigned short *buffer,int len){
    uint32_t sum=0;
    while (len > 1) {
        sum += *buffer++;
        len -= 2;
    }
    if (len > 0) {
        sum += *(unsigned char *)buffer;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    return (unsigned short)(~sum);
    





}

int main(){
    char buffer[2222];
    char rbuffer[3333];
    int d = socket(AF_PACKET,SOCK_RAW,0);
    if (d<0){
        perror("err1");
    }
    struct ethhdr * eth=(struct ethhdr *)buffer;
    struct ifreq req;
    strcpy(req.ifr_name,"wlo1");
    ioctl(d,SIOCGIFADDR,&req);
    struct sockaddr_in *ip=(struct sockaddr_in*)&req.ifr_addr;
    uint32_t myip=ip->sin_addr.s_addr;
    struct sockaddr_in *netmask;
    ioctl(d,SIOCGIFNETMASK,&req);
    netmask=(struct sockaddr_in*)&req.ifr_addr;
    uint32_t mask=netmask->sin_addr.s_addr;
    struct in_addr broadcast;
    struct in_addr network;
    network.s_addr=myip&mask;
    broadcast.s_addr=network.s_addr  |~mask;
    uint32_t net=network.s_addr;
    uint32_t brd=broadcast.s_addr;
    uint32_t net_h = ntohl(net);     
    uint32_t brd_h = ntohl(brd);    





    struct iphdr *ipn=(struct iphdr *)(buffer+sizeof(struct ethhdr));
    ipn->ihl = 5;
    ipn->version = 4;
    ipn->tos = 0;
    ipn->tot_len =htons(sizeof(struct iphdr)+sizeof(struct tcphdr));
    ipn->id = htons(1);
    ipn->frag_off = 0;
    ipn->ttl = 64;
    ipn->protocol = IPPROTO_TCP;
    ipn->saddr = myip;
    struct tcphdr *tcp=(struct tcphdr *)(buffer +sizeof(struct ethhdr)+sizeof(struct iphdr));
    tcp->source=htons(179);
    tcp->dest=
    tcp->seq=htons(0);
    tcp->ack_seq=htons(0);
    tcp->doff=5;
    tcp->syn=1;
    //tcp->ack
    //tcp->fin
    //tcp->rst
    //tcp->psh
    //tcp->urg
    tcp->window=htons(65535);
    //tcp->check
    //tcp->urg_ptr*/
    struct sockaddr_ll device;
    int binded=bind(d,(struct sockaddr*)&device,sizeof(device));
    if (binded<0){
       perror("not binded");
       return 1;
    }
    printf("Scanning from %u to %u (%d addresses)\n", net_h, brd_h, brd_h - net_h);//stylish
    
    for(uint32_t i=net_h+1;i<brd_h;i++){
        ipn->daddr = htonl(i);
         ipn->check = 0;
         ipn->check = checksum((unsigned short *)ipn,sizeof(struct iphdr));
         tcp->check=checksum(unsigned short*)tcp +sizeof(struct tcphdr);
        sendto(d,buffer,sizeof(struct ethhdr) + sizeof(struct iphdr)+ sizeof(struct icmphdr), 0, (struct sockaddr *)&device, sizeof(device));
        recvfrom(d, rbuffer, sizeof(rbuffer), 0, NULL, NULL);
        
   
}
}