#include "tcp_ipv4_socket.h"
#include "tcp_flags.h"

int main() {
    struct tcp_ip_socket* s = generate_tcp_ipv4_socket(
                              "192.168.1.2", "91.198.36.48", 47187, 110);
    ssize_t size;
    set_flags(s->datagram, 0, 1, 0, 0, 0, 0);
    size = send_datagram(s);
    printf("Size is %d\n", (int)size);
    close(s->socket);
    free(s);
    exit(EXIT_SUCCESS);
}
