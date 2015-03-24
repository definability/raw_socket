#ifndef TCP_IPV4SOCKET_H
#define TCP_IPV4SOCKET_H

#include<stdio.h>       // For printf
#include<string.h>      // memset
#include<sys/socket.h>  // For socket ofcourse
#include<stdlib.h>      // For exit(0);
#include<errno.h>       // For errno - the error number
#include<netinet/tcp.h> // Provides declarations for tcp header
#include<netinet/ip.h>  // Provides declarations for ip header
#include<arpa/inet.h>   // For `inet_addr'

/**
 * Datagram maximum message size.
 */
#define MESSAGE_SIZE 4096

/**
 * TCP IP Datagram structure.
 * Needed to hold messages and responses.
 */
struct tcp_ip_datagram {
    /**
     * IP Header.
     */
    struct iphdr iph;
    /**
     * TCP Header.
     */
    struct tcphdr tcph;
    /**
     * String with Message.
     */
    char message[MESSAGE_SIZE];
};

/**
 * TCP IP Socket structure.
 */
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
     * TCP IP Datagram.
     */
    struct tcp_ip_datagram* datagram;
};
/**
 * Generate socket with TCP and IPv4 protocols (Constructor).
 * @param source_ip Source IP address.
 * @param dest_ip Destination IP address.
 * @param source_port Source port.
 * @param dest_port Destination port.
 */
struct tcp_ip_socket* generate_tcp_ipv4_socket (
                              const char* source_ip, const char* dest_ip,
                              short source_port, short dest_port);
/**
 * Calculate checksums for IP and TCP headers.
 * @param sit TCP IP Socket which headers should be checksummed.
 */
int calculate_checksums(struct tcp_ip_socket* sit);
/**
 * Generate IP Socket Address.
 * @param dest_ip Destination IP address.
 * @param dest_port Destination port.
 */
struct sockaddr_in* generate_sockaddr (const char* dest_ip, short dest_port);
/**
 * Generate TCP IP Datagram (Constructor).
 * @param sin IP socket address (for checksum).
 * @param source_ip Source IP address.
 * @param dest_ip Destination IP address.
 * @param source_port Source port.
 * @param dest_port Destination port.
 */
struct tcp_ip_datagram* generate_tcp_ip_datagram (struct sockaddr_in* sin,
                              const char* source_ip, const char* dest_ip,
                              short source_port, short dest_port);
/**
 * Generate IP version 4 headers.
 * @param iph Not NULL IP Header pointer to hold new instance.
 * @param source_ip Source IP address.
 * @param dest_ip Destination IP address.
 */
int generate_ipv4_hdr (struct iphdr* iph,
                       const char* source_ip, const char* dest_ip);
 /**
 * Generate TCP headers.
 * @param tcph Not NULL TCP Header pointer to hold new instance.
 * @param source_port Source port.
 * @param dest_port Destination port.
 * @param sin IP socket address (for checksum).
 */
int generate_tcp_hdr (struct tcphdr* tcph, short source_port, short dest_port);
/**
 * Send datagram binded to socket.
 */
ssize_t send_datagram(struct tcp_ip_socket* tis);
/**
 * Receive new message to socket.
 */
struct tcp_ip_datagram* recieve_datagram(struct tcp_ip_socket* tis);

#endif
