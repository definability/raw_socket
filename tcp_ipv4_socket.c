#include "tcp_ipv4_socket.h"

#define MESSAGE_SIZE 4096

struct tcp_ip_socket {
    /**
     * Socket descriptor.
     */
    int socket;
    /**
     * IP socket address.
     */
    struct sockaddr_in* sin;
    /**
     * IP Header.
     */
    struct iphdr* iph;
    /**
     * TCP Header.
     */
    struct tcphdr* tcph;
    /**
     * String with Message.
     */
    char message[MESSAGE_SIZE];
};

int generate_tcp_ipv4_socket (struct tcp_ip_socket* sit,
                              const char* source_ip, const char* dest_ip,
                              const char* source_port, const char* dest_port) {
    if (!sit) {
        return -1;
    }
    return 0;
}
