#ifndef CHECKSUM_H
#define CHECKSUM_H

#include<netinet/tcp.h> // Provides declarations for tcp header
#include<netinet/ip.h>  // Provides declarations for ip header
#include "tcp_ipv4_socket.h"

/**
 * 96 bit (12 bytes) pseudo header needed for tcp header checksum calculation.
 */
struct pseudo_header;
/**
 * Checksum function.
 * @param ptr Pointer to memory block which should be checksummed.
 * @param nbytes Size of the block.
 * @return Returns checksum of the block.
 */
unsigned short csum(unsigned short *ptr,int nbytes);
/**
 * Calculate checksum for IP version 4 Header in datagram.
 * @param datagram Datagram which should be analyzed.
 */
int checksum_ipv4_hdr (struct tcp_ip_datagram* datagram);
/**
 * Calculate checksum for TCP Header in datagram.
 * @param datagram Datagram which should be analyzed.
 */
int checksum_tcp_hdr (struct tcp_ip_datagram* datagram);
/**
 * Calculate checksum for TCP and IP version 3 headers in Datagram.
 * @param datagram Datagram which should be analyzed.
 */
int checksum_datagram (struct tcp_ip_datagram* datagram);

#endif
