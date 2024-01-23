#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <net/ethernet.h>
#include <errno.h>
#include <stdio.h>

#include "./common.h"

int ip_raw_value(int af, const char *src, void *dest, unsigned int size)
{
    unsigned char buf[sizeof(struct in6_addr)];
    int s;
    char str[INET6_ADDRSTRLEN];

    s = inet_pton(af, src, buf);

    if (s <= 0)
        return EXIT_FAILURE;

    if (inet_ntop(af, buf, str, INET6_ADDRSTRLEN) == NULL)
        return EXIT_FAILURE;

    memcpy(dest, buf, size);

    return EXIT_SUCCESS;
}

unsigned int ip4_raw_value(const char *src)
{
    unsigned int ret = 0;

    if (!src)
        return 0;

    ip_raw_value(AF_INET, src, &ret, sizeof(ret));

    return ntohl(ret);
}

int ip_string_value(int af, const char *buf, char *dest)
{
    if (inet_ntop(af, buf, dest, INET6_ADDRSTRLEN) == NULL) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int eth_raw_value(const char *src, unsigned char *dest)
{
    int len;

    if (!src || !dest)
        return EXIT_FAILURE;

    len = sscanf(src, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
           &dest[0], &dest[1], &dest[2],
           &dest[3], &dest[4], &dest[5]);
    
    if (len != ETH_ALEN)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int eth_string_value(unsigned char *src, char *dest)
{
    int len;
    
    if (!src || !dest)
        return EXIT_FAILURE;

    len = snprintf(dest, ETH_STR_LEN, "%02x:%02x:%02x:%02x:%02x:%02x",
        src[0], src[1], src[2],
        src[3], src[4], src[5]);

    if (len != ETH_STR_LEN - 1)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

unsigned short port_raw_value(const char *src)
{
    int len;
    unsigned short ret = 0;

    if (!src)
        return 0;

    len = sscanf(src, "%hu", &ret);
    if (len != 1)
        return 0;
    

    return ret;
}
