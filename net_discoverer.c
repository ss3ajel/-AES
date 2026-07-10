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
int main()
{
    unsigned char buffer[2333];

    unsigned char broadcast_mac[6] =
    {
        0xff,0xff,0xff,0xff,0xff,0xff
    };


    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    struct ifreq ifr;

    memset(&ifr,0,sizeof(ifr));

    strcpy(ifr.ifr_name,"eth0");
    ioctl(fd,SIOCGIFINDEX,&ifr);
    int ifindex = ifr.ifr_ifindex;
    ioctl(fd,SIOCGIFHWADDR,&ifr);

    unsigned char *my_mac =
        (unsigned char *)ifr.ifr_hwaddr.sa_data;
    ioctl(fd,SIOCGIFADDR,&ifr);

    struct sockaddr_in *ipaddr =
        (struct sockaddr_in *)&ifr.ifr_addr;


    uint32_t my_ip = ipaddr->sin_addr.s_addr;

    struct ethhdr *eth =
        (struct ethhdr *)buffer;


    memcpy(eth->h_dest,broadcast_mac,6);

    memcpy(eth->h_source,my_mac,6);

    eth->h_proto = htons(ETH_P_IP);

    struct iphdr *ip =
        (struct iphdr *)(buffer + sizeof(struct ethhdr));


    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len =
        htons(sizeof(struct iphdr)+sizeof(struct icmphdr));
    ip->id = htons(1);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_ICMP;
    ip->saddr = my_ip;
    ip->daddr = inet_addr("255.255.255.255");
    ip->check = checksum(
        (unsigned short *)ip,
        sizeof(struct iphdr)
    );
    struct icmphdr *icmp =
        (struct icmphdr *)(buffer
        + sizeof(struct ethhdr)
        + sizeof(struct iphdr));
    icmp->type = ICMP_ECHO;

    icmp->code = 0;
    icmp->un.echo.id = htons(1234);
    icmp->un.echo.sequence = htons(1);
    icmp->checksum =
        checksum(
        (unsigned short *)icmp,
        sizeof(struct icmphdr)
        );

    struct sockaddr_ll device;

    memset(&device,0,sizeof(device));

    device.sll_family = AF_PACKET;

    device.sll_ifindex = ifindex;

    device.sll_halen = ETH_ALEN;

    memcpy(device.sll_addr,
           broadcast_mac,
           6);



    sendto(
        fd,
        buffer,
        sizeof(struct ethhdr)
        + sizeof(struct iphdr)
        + sizeof(struct icmphdr),
        0,
        (struct sockaddr *)&device,
        sizeof(device)
    );


    close(fd);

    return 0;
}