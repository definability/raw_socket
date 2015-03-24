#ifndef TCP_COMMANDS_H
#define TCP_COMMANDS_H

#include "../socket/tcp_ipv4_socket.h"

/**
 * Establishes TCP connection with 3-way handshake.
 */
int tcp_handshake (struct tcp_ip_socket* s);
/**
 * Reset TCP connection.
 */
int tcp_reset (struct tcp_ip_socket* s, struct tcp_ip_datagram* response);
/**
 * Close TCP connection.
 */
int tcp_close (struct tcp_ip_socket* s);
/**
 * Finish TCP connection closing (after server FIN message).
 */
int tcp_fin (struct tcp_ip_socket* s, struct tcp_ip_datagram* response);
/**
 * Confirm received message.
 */
int tcp_confirm (struct tcp_ip_socket* s);
/**
 * Send message to server and set PSH flag to 1.
 */
int tcp_push_message (struct tcp_ip_socket* s, const char* message);

#endif
