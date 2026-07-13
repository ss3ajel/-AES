#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <poll.h>
#include<time.h>
#include<sys/time.h>
//the goal is mimicking the nmap command//
struct device {
    char ip[16];
    char mac[18];
    time_t last_seen;
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

int main()
{
    unsigned char buffer[2333];
    unsigned char nbuffer[5555];

    unsigned char broadcast_mac[6] =
    {
        0xff,0xff,0xff,0xff,0xff,0xff
    };

     struct timeval t;
    t.tv_sec = 0;
    t.tv_usec = 300000;// isana 300ms

    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));
    struct ifreq ifr;

    memset(&ifr,0,sizeof(ifr));

    strcpy(ifr.ifr_name,"eth0");
    ioctl(fd,SIOCGIFINDEX,&ifr);
    int ifindex = ifr.ifr_ifindex;
    ioctl(fd,SIOCGIFHWADDR,&ifr);

    unsigned char *my_mac =(unsigned char *)ifr.ifr_hwaddr.sa_data;
    ioctl(fd,SIOCGIFADDR,&ifr);

     

    struct sockaddr_in *ipaddr =
        (struct sockaddr_in *)&ifr.ifr_addr;    
    uint32_t my_ip = ipaddr->sin_addr.s_addr;
    struct sockaddr_in *netmask_addr;
    ioctl(fd,SIOCGIFNETMASK,&ifr);
    netmask_addr = (struct sockaddr_in *)&ifr.ifr_addr;
    uint32_t mask=netmask_addr->sin_addr.s_addr;
    struct in_addr network ;
    struct in_addr broadcast ;
    network.s_addr=my_ip&mask;
    broadcast.s_addr=network.s_addr|mask;
    uint32_t net=network.s_addr;
    uint32_t brd=broadcast.s_addr;
    uint32_t net_h = ntohl(net);     
    uint32_t brd_h = ntohl(brd);    



    struct ethhdr *eth =(struct ethhdr *)buffer;


    memcpy(eth->h_dest,broadcast_mac,6);

    memcpy(eth->h_source,my_mac,6);

    eth->h_proto = htons(ETH_P_IP);

    struct iphdr *ip =(struct iphdr *)(buffer + sizeof(struct ethhdr));
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len =htons(sizeof(struct iphdr)+sizeof(struct icmphdr));
    ip->id = htons(1);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_ICMP;
    ip->saddr = my_ip;
   
    
    struct icmphdr *icmp =(struct icmphdr *)(buffer+ sizeof(struct ethhdr) + sizeof(struct iphdr));
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = htons(1234);
    
    
   
    struct sockaddr_ll device;

    memset(&device,0,sizeof(device));

    device.sll_family = AF_PACKET;

    device.sll_ifindex = ifindex;

    device.sll_halen = ETH_ALEN;

    memcpy(device.sll_addr, broadcast_mac, 6);
    struct device d[20];//lets say i have lesss than 20 devices
    int j=0;
    
    for(uint32_t i=net+1;i<brd-1;i++){
         ip->daddr = i;
         ip->check = checksum((unsigned short *)ip,sizeof(struct iphdr));
         icmp->un.echo.sequence = htons(1);
         icmp->checksum =checksum((unsigned short *)icmp, sizeof(struct icmphdr));

        sendto(fd,buffer,sizeof(struct ethhdr) + sizeof(struct iphdr)+ sizeof(struct icmphdr), 0, (struct sockaddr *)&device, sizeof(device));
         socklen_t len=sizeof(device);
         int rec_len=recvfrom(fd,nbuffer,sizeof(nbuffer),0,(struct sockaddr*)&device,&len);
         
        
       
        if(rec_len<sizeof(struct ethhdr)) {continue;}//14 bytes}
        struct ethhdr *reth=(struct ethhdr*)nbuffer;
        
       
        if (ntohs(reth->h_proto) != 0x0800) { continue; }
        if(rec_len<sizeof(struct ethhdr )+sizeof(struct iphdr)){continue;}
        struct iphdr *rip=(struct iphdr*) (nbuffer+sizeof(struct ethhdr));//+20 bytes
        
        if (rip->protocol != IPPROTO_ICMP) { continue; } // 8bytes
        size_t ip_hd_len=rip->ihl*4;
       
        if (ip_hd_len < 20 || rec_len < sizeof(struct ethhdr) + ip_hd_len + sizeof(struct icmphdr)) { 
        continue; 
}
        struct icmphdr *ricmp=(struct icmphdr*)(nbuffer+sizeof(struct ethhdr )+ip_hd_len);
      
      
        if(ricmp->type==ICMP_ECHOREPLY&&ricmp->un.echo.id==ntohs(1234)){
           
            while(j<20){
                sprintf(d[j].mac, "%02x:%02x:%02x:%02x:%02x:%02x",reth->h_source[0],
                reth->h_source[1],reth->h_source[2],reth->h_source[3],reth->h_source[4],reth->h_source[5]);
                unsigned char *ip = (unsigned char *)&rip->saddr;
                sprintf(d[j].ip, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
                d[j].last_seen = time(NULL);
                printf("%ld", (long)d[j].last_seen);
                j++;
            }
                
            

           
            
          




        }

    }
     
    
    


    close(fd);

    return 0;
}
