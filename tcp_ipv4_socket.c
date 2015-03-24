#include "tcp_ipv4_socket.h"

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
     * Datagram (workaround).
     * Contains (exactly in this order):
     *      - struct iphdr*     IP Header
     *      - struct tcphdr*    TCP Header
     *      - char*             Data to send
     * Needed to receive server answers and parse them.
     * Reason: struct can have padding between fields, and datagram should not
     * to have it.
     */
    char* datagram;
    /** 
     **********************CAUTION**********************
     * To  obtain   further  fields   you   must   call*
     * `int parse_datagram(struct tcp_ip_socket* tip)'.*
     * Otherwise they can (and wil) contain wrong data.*
     *++++++++++++++++++++++ALSO+++++++++++++++++++++++*
     * After  changing  their  values  you  must   call*
     * `int save_datagram(struct tcp_ip_socket*  tip)'.*
     * Otherwise  `datagram'  will  contain wrong data.*
     ************************END************************
     */
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
    char* message;
};

int generate_tcp_ipv4_socket (struct tcp_ip_socket* sit,
                              const char* source_ip, const char* dest_ip,
                              const char* source_port, const char* dest_port) {
    if (!sit) {
        return -1;
    }
    return 0;
}
