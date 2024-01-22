#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

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
