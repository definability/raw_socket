#include "tcp_commands.h"
#include "tcp_messaging.h"
#include "tcp_flags.h"

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
    memset(s->datagram->message, 0, MESSAGE_SIZE);
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
        //response = receive_datagram(s);
        int increment = response->tcph.psh? strlen(response->message) : 1;
        _set_tcp_confirm(s->datagram, response, increment);
        set_flags(s->datagram, 1, 0, 0, 0, 1, 0);
    }
    else if (response->tcph.ack && !response->tcph.fin){
        _set_tcp_confirm(s->datagram, response, 1);
        set_flags(s->datagram, 0, 0, 0, 0, 1, 0);
        send_datagram(s);
    }
    if (send_datagram(s) < 0) {
        return -1;
    }
    return 0;
}

int tcp_confirm (struct tcp_ip_socket* s) {
    struct tcp_ip_datagram* response;
    if (!s) {
        fprintf(stderr, "Can't confirm TCP message with NULL Socket pointer\n");
        return -1;
    }
    response = receive_datagram(s);
    if (response->tcph.fin) {
        if (tcp_fin(s, response) != 0) {
            free(response);
            return -1;
        }
    }
    else {
        printf("%s", response->message);
        set_tcp_confirm(s->datagram, response);
        send_datagram(s);
        free(response);
        return 0;
    }
    return 0;
}

int tcp_push_message (struct tcp_ip_socket* s, const char* message) {
    struct tcp_ip_datagram* response;
    memset(s->datagram->message, 0, MESSAGE_SIZE);
    strcpy(s->datagram->message, message);
    add_flags(s->datagram, 0, 0, 0, 1, 0, 0);
    if (send_datagram(s) < 0) {
        return -1;
    }
    // TODO: Check if answer is correct
    response = receive_datagram(s);
    if (!response) {
        fprintf(stderr, "Can't receive response from server\n");
        return -1;
    }
    free(response);
    return 0;
}
