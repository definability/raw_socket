#include "tcp_flags.h"
#include "checksum.h"

int set_flags(struct tcp_ip_datagram* datagram,
              int fin, int syn, int rst, int psh, int ack, int urg) {
    struct tcphdr* tcph;
    if (!datagram) {
        fprintf(stderr, "Can't set flags for TCP Header with "
                        "NULL Datagram pointer.\n");
        return -1;
    }
    tcph = &datagram->tcph;
    tcph->fin = fin;
    tcph->syn = syn;
    tcph->rst = rst;
    tcph->psh = psh;
    tcph->ack = ack;
    tcph->urg = urg;
    checksum_tcp_hdr(datagram);
    return 0;
}

int add_flags(struct tcp_ip_datagram* datagram,
              int fin, int syn, int rst, int psh, int ack, int urg) {
    struct tcphdr* tcph;
    if (!datagram) {
        fprintf(stderr, "Can't set flags for TCP Header with "
                        "NULL Datagram pointer.\n");
        return -1;
    }
    tcph = &datagram->tcph;
    tcph->fin |= fin;
    tcph->syn |= syn;
    tcph->rst |= rst;
    tcph->psh |= psh;
    tcph->ack |= ack;
    tcph->urg |= urg;
    checksum_tcp_hdr(datagram);
    return 0;
}

int del_flags(struct tcp_ip_datagram* datagram,
              int fin, int syn, int rst, int psh, int ack, int urg) {
    struct tcphdr* tcph;
    if (!datagram) {
        fprintf(stderr, "Can't set flags for TCP Header with "
                        "NULL Datagram pointer.\n");
        return -1;
    }
    tcph = &datagram->tcph;
    tcph->fin ^= fin;
    tcph->syn ^= syn;
    tcph->rst ^= rst;
    tcph->psh ^= psh;
    tcph->ack ^= ack;
    tcph->urg ^= urg;
    checksum_tcp_hdr(datagram);
    return 0;
}
