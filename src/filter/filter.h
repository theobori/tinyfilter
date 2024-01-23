#ifndef __FILTER_H__
#define __FILTER_H__

#include <linux/types.h>
#include <linux/if_ether.h>
#include <linux/ipv6.h>

/**
 * @brief Filter key ethernet
 * 
 */
typedef struct filter_key_eth_s {
	__u8 dst[ETH_ALEN];
	__u8 src[ETH_ALEN];
} filter_key_eth_t;

/**
 * @brief Filter key ip
 * 
 */
typedef struct filter_key_ip_s {
	__u32 dst;
	__u32 src;
} filter_key_ip_t;

/**
 * @brief Filter key ip6
 * 
 */
typedef struct filter_key_ip6_s {
	__u8 dst[16];
	__u8 src[16];
} filter_key_ip6_t;

/**
 * @brief Filter key port (tcp + udp)
 * 
 */
typedef struct filter_key_port_s {
	__u16 dst;
	__u16 src;
    __u8 proto;
    __u8 unused[3];
} filter_key_port_t;

/**
 * @brief Filter key icmp
 * 
 */
typedef struct filter_key_icmp_s {
    __u8 type;
    __u8 unused[3];
} filter_key_icmp_t;

/**
 * @brief Filter value
 * 
 */
typedef struct filter_value_s {
    __u64 count;
} filter_value_t;

#endif
