#include "../utils/checksum.h"
#include "tcp_ipv4_socket.h"

struct tcp_ip_socket* generate_tcp_ipv4_socket (
                              const char* source_ip, const char* dest_ip,
                              short source_port, short dest_port) {
    struct tcp_ip_socket* sit;
    sit = (struct tcp_ip_socket*) malloc(sizeof(struct tcp_ip_socket));
    if (!sit) {
        fprintf(stderr, "Cannot allocate memory for socket pointer.\n");
        return NULL;
    }
    sit->socket = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sit->socket == -1)
    {
        perror("Failed to create socket");
        return NULL;
    }
    sit->sin = generate_sockaddr(dest_ip, dest_port);
    if (!sit->sin) {
        free(sit);
        return NULL;
    }
    sit->datagram = generate_tcp_ip_datagram(sit->sin, source_ip, dest_ip,
                                             source_port, dest_port);
    if (!sit->datagram) {
        free(sit->sin);
        free(sit);
        return NULL;
    }
    //IP_HDRINCL to tell the kernel that headers are included in the packet
    int one = 1;
    if (setsockopt (sit->socket, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0)
    {
        perror("Error setting IP_HDRINCL");
        return NULL;
    }
    return sit;
}

struct sockaddr_in* generate_sockaddr (const char* dest_ip, short dest_port) {
    struct sockaddr_in* sin;
    sin = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    if (!sin) {
        fprintf(stderr, "Cannot allocate memory for IP socket address.\n");
        return NULL;
    }
    sin->sin_addr.s_addr = inet_addr(dest_ip);
    sin->sin_port = htons(dest_port);
    sin->sin_family = AF_INET;
    return sin;
}

struct tcp_ip_datagram* generate_tcp_ip_datagram (struct sockaddr_in* sin,
                              const char* source_ip, const char* dest_ip,
                              short source_port, short dest_port) {
    struct tcp_ip_datagram* datagram;
    if (!sin) {
        fprintf(stderr, "IP socket address pointer can not be NULL "
                        "for Datagram construction.\n");
        return NULL;
    }
    datagram = (struct tcp_ip_datagram*) malloc(sizeof(struct tcp_ip_datagram));
    if (!datagram) {
        fprintf(stderr, "Cannot allocate memory for datagram.\n");
        return NULL;
    }
    if (generate_tcp_hdr(&(datagram->tcph), source_port, dest_port) != 0) {
        free(datagram);
        return NULL;
    }
    if (generate_ipv4_hdr(&(datagram->iph), source_ip, dest_ip) != 0) {
        free(datagram);
        return NULL;
    }
    if (checksum_datagram(datagram) != 0) {
        free(datagram);
        return NULL;
    }
    return datagram;
}

int generate_ipv4_hdr (struct iphdr* iph,
                       const char* source_ip, const char* dest_ip) {
    if (!iph) {
        fprintf(stderr, "IP Header cannot be NULL.\n");
        return -1;
    }
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct tcphdr);
    iph->id = htonl (54321); // ID of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0;                       //Set to 0 before calculating checksum
    iph->saddr = inet_addr(source_ip); //Spoof the source ip address
    iph->daddr = inet_addr(dest_ip);
    return 0;
}

int generate_tcp_hdr (struct tcphdr* tcph,
                      short source_port, short dest_port) {
    if (!tcph) {
        fprintf(stderr, "TCP Header cannot be NULL.\n");
        return -1;
    }
    tcph->source = htons(source_port);
    tcph->dest = htons (dest_port);
    tcph->seq = rand() & 0xffff;
    tcph->ack_seq = 0;
    tcph->doff = 5;  //tcp header size
    tcph->window = htons(1000); //htons (5840); /* maximum allowed window size */
    tcph->check = 0; //leave checksum 0 now, filled later by pseudo header
    tcph->urg_ptr = 0;
    return 0;
}

ssize_t send_datagram(struct tcp_ip_socket* sit) {
    checksum_datagram(sit->datagram);
    if (sendto(sit->socket, sit->datagram, sit->datagram->iph.tot_len, 0,
                  (struct sockaddr*) sit->sin, sizeof(*(sit->sin))) < 0) {
        perror("Can't send datagram");
        return -1;
    }
    return 0;
}

struct tcp_ip_datagram* receive_datagram(struct tcp_ip_socket* sit) {
    struct tcp_ip_datagram* response;
    response = (struct tcp_ip_datagram*) malloc(sizeof(struct tcp_ip_datagram));
    memset(response->message, 0, MESSAGE_SIZE);
    recvfrom(sit->socket, (void*) response, sizeof(*response),
             0, NULL, NULL);
    return response;
}
