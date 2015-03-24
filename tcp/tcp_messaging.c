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

int tcp_handshake (struct tcp_ip_socket* s) {
    struct tcp_ip_datagram* response;
    if (!s) {
        fprintf(stderr, "Can't connect Socket with NULL pointer\n");
        return -1;
    }
    // SYN
    set_flags(s->datagram, 0, 1, 0, 0, 0, 0);
    if (send_datagram(s) < 0) {
        return -1;
    }
    // ACK
    response = receive_datagram(s);
    set_tcp_seq_ack(s->datagram, ntohl(response->tcph.th_ack),
                                 ntohl(response->tcph.seq) + 1);
    free(response);
    if (send_datagram(s) < 0) {
        return -1;
    }
    return 0;
}

int set_tcp_reset (struct tcp_ip_datagram* datagram,
                   struct tcp_ip_datagram* response) {
    if (!datagram || !response) {
        fprintf(stderr, "Can't reset message with NULL "
                        "Datagrams' pointers\n");
        return -1;
    }
    set_tcp_seq_ack(datagram, ntohl(response->tcph.th_ack),
                    ntohl(response->tcph.seq) + strlen(response->message));
    set_flags(datagram, 0, 0, 1, 0, 0, 0);
    return 0;
}

int tcp_close (struct tcp_ip_socket* s) {
    struct tcp_ip_datagram* response;
    if (!s) {
        fprintf(stderr, "Can't close TCP Connection with "
                        "NULL Socket pointer\n");
        return -1;
    }
    set_flags(s->datagram, 1, 0, 0, 0, 1, 0);
    if (send_datagram(s) < 0) {
        return -1;
    }
    response = receive_datagram(s);
    if (tcp_fin(s, response) < 0) {
        return -1;
    }
    return 0;
}

int tcp_fin (struct tcp_ip_socket* s, struct tcp_ip_datagram* response) {
    if (!s) {
        fprintf(stderr, "Can't finish TCP Connection with "
                        "NULL Socket pointer\n");
        return -1;
    }
    else if (!response) {
        fprintf(stderr, "Can't finish TCP Connection without "
                        "response from server\n");
        return -1;
    }
    if (response->tcph.ack && response->tcph.fin) {
        response = receive_datagram(s);
        /*
        _set_tcp_confirm(s->datagram, response, 1);
        set_flags(s->datagram, 0, 0, 0, 0, 1, 0);
        */
    }
    if (response->tcph.ack && !response->tcph.fin){
        _set_tcp_confirm(s->datagram, response, 1);
        set_flags(s->datagram, 0, 0, 0, 0, 1, 0);
        send_datagram(s);
    }
    if (send_datagram(s) < 0) {
        return -1;
    }
    return 0;
}
