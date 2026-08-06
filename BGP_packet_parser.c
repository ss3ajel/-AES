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
struct pseudo_header {
    uint32_t src_addr;
    uint32_t dst_addr;
    uint8_t  reserved;
    uint8_t  protocol;
    uint16_t tcp_length;
};
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
     unsigned char broadcast_mac[6] =
    {
        0xff,0xff,0xff,0xff,0xff,0xff
    };

    int d = socket(AF_PACKET,SOCK_RAW,0);
    if (d<0){
        perror("err1");
    }
    struct ethhdr * eth=(struct ethhdr *)buffer;
    memcpy(eth->h_dest,broadcast_mac,6);
    struct ifreq req;
    strcpy(req.ifr_name,"wlo1");
    ioctl(d,SIOCGIFHWADDR,&req);
    unsigned char *my_mac =(unsigned char *)req.ifr_hwaddr.sa_data;
    memcpy(eth->h_source,my_mac,6);
    eth->h_proto = htons(ETH_P_IP);

    ioctl(d,SIOCGIFINDEX,&req);
    int ifindex = req.ifr_ifindex;
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
    tcp->source=htons(54321);
    tcp->dest=htons(179);
    tcp->seq=htonl(0);
    tcp->ack_seq=htonl(0);
    tcp->doff=5;
    tcp->syn=1;
    //tcp->ack
    //tcp->fin
    //tcp->rst
    //tcp->psh
    //tcp->urg
    tcp->window=htons(65535);


    struct sockaddr_ll device;
    memset(&device,0,sizeof(device));

    device.sll_family = AF_PACKET;

    device.sll_ifindex = ifindex;

    device.sll_halen = ETH_ALEN;
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
         tcp->check=0;
         char pbuffer[3333];
         struct pseudo_header *pseudo=(struct pseudo_header*)(pbuffer);
        pseudo->src_addr = ipn->saddr;
       pseudo->dst_addr = ipn->daddr;
         pseudo->protocol = IPPROTO_TCP;
        pseudo->tcp_length = htons(sizeof(struct tcphdr) + sizeof(struct bgp_header));
        
         
        uint8_t *payload = (uint8_t *)tcp + tcp->doff * 4;
         tcp=(struct tcphdr*)(pbuffer+sizeof(struct pseudo_header));


         tcp->check=checksum((unsigned short*)tcp ,sizeof(pseudo));

        sendto(d,buffer,sizeof(struct ethhdr) + sizeof(struct iphdr)+ sizeof(struct tcphdr), 0, (struct sockaddr *)&device, sizeof(device));
        struct sockaddr_ll receiver;
         receiver.sll_family=AF_PACKET;
         receiver.sll_ifindex = ifindex;
         receiver.sll_halen = ETH_ALEN;
         socklen_t len=sizeof( receiver);
         

         int rec_len=recvfrom(d,rbuffer,sizeof(rbuffer),0,(struct sockaddr*)&receiver,&len);
         if(rec_len<sizeof(struct ethhdr)) {continue;}//14 bytes}
        struct ethhdr *reth=(struct ethhdr*)rbuffer;
        
       
        if (ntohs(reth->h_proto) != 0x0800) { continue; }
        if(rec_len<sizeof(struct ethhdr )+sizeof(struct iphdr)){continue;}
        struct iphdr *rip=(struct iphdr*) (rbuffer+sizeof(struct ethhdr));//+20 bytes
        
        if (rip->protocol != IPPROTO_TCP) { continue; } // 8bytes
        size_t ip_hd_len=rip->ihl*5;
       
        if (ip_hd_len < 20 || rec_len < sizeof(struct ethhdr) + ip_hd_len + sizeof(struct tcphdr)) { 
        continue; 
}
        struct tcphdr *rtcp=(struct tcphdr*)(rbuffer+sizeof(struct ethhdr )+ip_hd_len);

         uint8_t *payload = (uint8_t *)rtcp + rtcp->doff * 4;
         if(rtcp->ack&&rtcp->syn){
            //open
         }else (rtcp->rst){
            //close
         }
         struct bgp_header *bgp = (struct bgp_header *)payload;
         // if (port == 179 && has_payload) parse_bgp(payload);
}

}
