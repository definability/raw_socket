#ifndef TCP_FLAGS_H
#define TCP_FLAGS_H

#include "tcp_ipv4_socket.h"

/**
 * Set TCP header flags.
 */
int set_flags(struct tcp_ip_datagram* datagram,
              int fin, int syn, int rst, int psh, int ack, int urg);
/**
 * Add TCP header flags (does not unset flags which were set 1).
 */
int add_flags(struct tcp_ip_datagram* datagram,
              int fin, int syn, int rst, int psh, int ack, int urg);
/**
 * Delete TCP header flags: sets to 0 those flags, corresponding parameters of
 * which are set as 1.
 */
int del_flags(struct tcp_ip_datagram* datagram,
              int fin, int syn, int rst, int psh, int ack, int urg);

#endif
