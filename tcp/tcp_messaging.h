#ifndef TCP_MESSAGING_H
#define TCP_MESSAGING_H

#include "../socket/tcp_ipv4_socket.h"

/**
 * Set Sequence and Acknowledgement number for TCP Header in Datagram.
 * Also sets ACK flag to 1.
 * @param datagram Datagram which contains target TCP Header.
 * @param sequence New sequence number.
 * @param sequence New acknowledgement number.
 */
int set_tcp_seq_ack (struct tcp_ip_datagram* datagram,
                     long sequence, long acknowledgement);
/**
 * Set confirmation data to TCP Header in Datagram to respond to server.
 * @param datagram Source datagram with target TCP Header.
 * @param response Server response which needs to be confirmed.
 */
int set_tcp_confirm (struct tcp_ip_datagram* datagram,
                     struct tcp_ip_datagram* response);
/**
 * Set confirmation data to TCP Header in Datagram to respond to server
 * and manually choose increment of the sequence.
 * @param datagram Source datagram with target TCP Header.
 * @param response Server response which needs to be confirmed.
 */
int _set_tcp_confirm (struct tcp_ip_datagram* datagram,
              struct tcp_ip_datagram* response, long sequence_increment);

#endif
