#include "tcp_messaging.h"
#include "tcp_flags.h"

int set_tcp_seq_ack (struct tcp_ip_datagram* datagram,
                     long sequence, long acknowledgement) {
    if (!datagram) {
        fprintf(stderr, "Can't set sequence and acknowledgement for TCP header "
                        "with NULL pointer to Datagram.\n");
        return -1;
    }
    datagram->tcph.seq = htonl(sequence);
    datagram->tcph.th_ack = htonl(acknowledgement);
    set_flags(datagram, 0, 0, 0, 0, 1, 0);
    return 0;
}

int set_tcp_confirm (struct tcp_ip_datagram* datagram,
                     struct tcp_ip_datagram* response) {
    if (!datagram || !response) {
        fprintf(stderr, "Can't confirm message with NULL "
                        "Datagrams' pointers\n");
        return -1;
    }
    memset(datagram->message, 0, MESSAGE_SIZE);
    /*
    set_tcp_seq_ack(datagram, ntohl(response->tcph.th_ack),
                    ntohl(response->tcph.seq) + strlen(response->message));
                    */
    return _set_tcp_confirm(datagram, response, strlen(response->message));
}

int _set_tcp_confirm (struct tcp_ip_datagram* datagram,
              struct tcp_ip_datagram* response, long sequence_increment) {
    if (!datagram || !response) {
        fprintf(stderr, "Can't confirm response with NULL "
                        "Datagrams' pointers\n");
        return -1;
    }
    set_tcp_seq_ack(datagram, ntohl(response->tcph.th_ack),
                    ntohl(response->tcph.seq) + sequence_increment);
    return 0;
}
