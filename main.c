#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils/checksum.h"
#include "utils/addresses.h"
#include "socket/tcp_ipv4_socket.h"
#include "tcp/tcp_flags.h"
#include "tcp/tcp_messaging.h"
#include "tcp/tcp_commands.h"

int main(int argc, const char* argv[]) {
    srand(time(NULL));
    struct tcp_ip_socket* s;
    struct tcp_ip_datagram* response;
    int source_port;
    char *dest_ip;
    char dest_host[] = "pop.i.ua";

    if (argc < 2) {
        source_port = 47180;
    }
    else {
        source_port = atoi(argv[1]);
    }

    dest_ip = get_ip_address(dest_host);
    s = generate_tcp_ipv4_socket("192.168.1.2", dest_ip,
                                 source_port, 110);
    free(dest_ip);

    // HANDSHAKE
    if (tcp_handshake(s) < 0) {
        close(s->socket);
        free(s);
        exit(EXIT_FAILURE);
    }

    // ACK message
    if (tcp_confirm(s) < 0) {
        close(s->socket);
        free(s);
        exit(EXIT_FAILURE);
    }

    tcp_push_message(s, "USER guest\r\n");
    /*
    response = receive_datagram(s);
    set_tcp_confirm(s->datagram, response);
    */
    // ACK message
    if (tcp_confirm(s) < 0) {
        close(s->socket);
        free(s);
        exit(EXIT_FAILURE);
    }

    char* password = getpass("Enter password: ");
    char msg[120];
    strcpy(msg, "PASS ");
    strcat(msg, password);
    strcat(msg, "\r\n");
    free(password);
    tcp_push_message(s, msg);
    response = receive_datagram(s);
    set_tcp_confirm(s->datagram, response);

    tcp_push_message(s, "LIST\r\n");
    response = receive_datagram(s);
    printf("LIST: %s", response->message);
    set_tcp_confirm(s->datagram, response);

    tcp_push_message(s, "QUIT\r\n");
    // ACK message
    memset(s->datagram->message, 0, MESSAGE_SIZE);
    if (tcp_confirm(s) < 0) {
        close(s->socket);
        free(s);
        exit(EXIT_FAILURE);
    }

    // CLOSE
    if (tcp_close(s) < 0) {
        printf("Unable to close connection\n");
        close(s->socket);
        free(s);
        exit(EXIT_FAILURE);
    }
    close(s->socket);
    free(s);
    exit(EXIT_SUCCESS);
}
