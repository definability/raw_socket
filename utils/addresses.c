#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<errno.h>
#include<netdb.h>
#include<arpa/inet.h>

#include "addresses.h"

char* get_ip_address (const char* host) {
    struct hostent *he;
    struct in_addr **addr_list;
    char* ip_address;

    he = gethostbyname(host);
    if (!he) {
        herror("gethostbyname");
        return NULL;
    }
    ip_address = (char*) malloc(sizeof(char)*16);
    addr_list = (struct in_addr**) he->h_addr_list;
    strcpy(ip_address, inet_ntoa(*addr_list[0]) );
    return ip_address;
}
