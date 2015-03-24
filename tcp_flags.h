#ifndef TCP_FLAGS_H
#define TCP_FLAGS_H

#include "tcp_ipv4_socket.h"

/**
 * Set TCP header flags.
 */
int set_flags(struct tcp_ip_datagram* datagram,
              int fin, int syn, int rst, int psh, int ack, int urg);

#endif
