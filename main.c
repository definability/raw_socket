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
    char msg[120] = {0};

    if (argc < 2) {
        printf("Parameters: username [port]\n");
        exit (EXIT_FAILURE);
    }
    if (argc < 3) {
        source_port = 47180;
    }
    else {
        source_port = atoi(argv[2]);
    }


    char* password = getpass("Enter password: ");

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

    tcp_push_message(s, "USER krygin2\r\n");
    strcpy(msg, "USER");
    strcat(msg, argv[1]);
    strcat(msg, "\r\n");
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

    memset(msg, 0, 120);
    strcpy(msg, "PASS ");
    strcat(msg, password);
    strcat(msg, "\r\n");
    free(password);
    tcp_push_message(s, msg);
    response = receive_datagram(s);
    printf("%s", response->message);
    set_tcp_confirm(s->datagram, response);

    tcp_push_message(s, "LIST\r\n");
    response = receive_datagram(s);
    printf("LIST: %s", response->message);
    set_tcp_confirm(s->datagram, response);

    tcp_push_message(s, "TOP 1 0\r\n");
    response = receive_datagram(s);
    printf("TOP: %s", response->message);
    set_tcp_confirm(s->datagram, response);

    tcp_push_message(s, "TOP 2 0\r\n");
    response = receive_datagram(s);
    printf("TOP: %s", response->message);
    set_tcp_confirm(s->datagram, response);

    tcp_push_message(s, "TOP 3 0\r\n");
    response = receive_datagram(s);
    printf("TOP: %s", response->message);
    set_tcp_confirm(s->datagram, response);

    tcp_push_message(s, "TOP 3 0\r\n");
    response = receive_datagram(s);
    printf("TOP: %s", response->message);
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
