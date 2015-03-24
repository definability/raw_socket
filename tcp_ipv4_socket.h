#include<stdio.h> //for printf
#include<string.h> //memset
#include<sys/socket.h>    //for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header

/**
 * TCP IP Socket structure.
 */
struct tcp_ip_socket;
/**
 * Generate socket with TCP and IPv4 protocols (Constructor).
 * @param sit Not NULL TCP IP Socket pointer to hold new instance.
 * @param source_ip Source IP address.
 * @param dest_ip Destination IP address.
 * @param source_port Source port.
 * @param dest_port Destination port.
 */
int generate_tcp_ipv4_socket (struct tcp_ip_socket* sit,
                              const char* source_ip, const char* dest_ip,
                              const char* source_port, const char* dest_port);
/**
 * Generate IP version 4 headers.
 * @param iph Not NULL IP Header pointer to hold new instance.
 * @param source_ip Source IP address.
 * @param dest_ip Destination IP address.
 */
int generate_ipv4_hdr (struct iphdr* iph, const char* source_ip,
                                         const char* dest_ip);
 /**
 * Generate TCP headers.
 * @param tcph Not NULL TCP Header pointer to hold new instance.
 * @param source_port Source port.
 * @param dest_port Destination port.
 * @param sin IP socket address (for checksum).
 */
int generate_tcp_hdr (struct tcphdr* tcph, const char* source_port,
                      const char* dest_port, struct sockaddr_in* sin);
