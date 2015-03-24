#include "checksum.h"

struct pseudo_header {
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};

int checksum_ipv4_hdr (struct tcp_ip_datagram* datagram) {
    struct iphdr* iph;

    if (!datagram) {
        fprintf(stderr, "Datagram cannot be NULL if you want to checksum its "
                        "IP header.\n");
        return -1;
    }

    iph = &(datagram->iph);
    iph->check = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr)
                   + strlen(datagram->message);
    iph->check = csum((unsigned short*)datagram, iph->tot_len);
    return 0;
}

int checksum_tcp_hdr (struct tcp_ip_datagram* datagram) {
    struct tcphdr* tcph;
    struct iphdr* iph;
    struct pseudo_header psh;
    char* pseudogram;

    if (!datagram) {
        fprintf(stderr, "Datagram cannot be NULL if you want to checksum its "
                        "TCP header.\n");
        return -1;
    }

    tcph = &(datagram->tcph);
    iph = &(datagram->iph);
    tcph->check = 0;

    psh.source_address = iph->saddr;
    psh.dest_address = iph->daddr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(datagram->message));

    pseudogram = malloc(iph->tot_len);
    memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), tcph,
           sizeof(struct tcphdr) + strlen(datagram->message));

    tcph->check = csum( (unsigned short*) pseudogram , iph->tot_len);

    free(pseudogram);
    return 0;
}

unsigned short csum(unsigned short *ptr,int nbytes) {
    register long sum;
    unsigned short oddbyte;
    register short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     
    return(answer);
}
