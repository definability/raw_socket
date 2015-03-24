#include "tcp_ipv4_socket.h"
#include "tcp_flags.h"
#include "tcp_messaging.h"

int main() {
    struct tcp_ip_socket* s = generate_tcp_ipv4_socket(
                              "192.168.1.2", "91.198.36.48", 47189, 110);
    struct tcp_ip_datagram* response;

    // HANDSHAKE
    if (tcp_handshake(s) < 0) {
        close(s->socket);
        free(s);
        exit(EXIT_FAILURE);
    }

    // ACK message
    response = receive_datagram(s);
    printf("Message %s, %d\n", response->message, strlen(response->message));
    set_tcp_confirm(s->datagram, response);
    send_datagram(s);

    // ACK message
    free(response);
    response = receive_datagram(s);
    printf("Message %s, %d\n", response->message, strlen(response->message));
    set_tcp_confirm(s->datagram, response);
    send_datagram(s);

    // CLOSE
    if (tcp_close(s) < 0) {
        close(s->socket);
        free(s);
        exit(EXIT_FAILURE);
    }

    close(s->socket);
    free(s);
    free(response);
    exit(EXIT_SUCCESS);
}
