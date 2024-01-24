#include <linux/bpf.h>

#include <string.h>

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#include <xdp/parsing_helpers.h>

#include "./filter/filter.h"

/**
 * @brief Max entry amount
 * 
 */
#define MAP_ENTRY_AMOUNT 128 

#define MAP_CREATE(map_name, key_name) \
struct { \
	__uint(type, BPF_MAP_TYPE_HASH); \
	__type(key, key_name); \
	__type(value, filter_value_t); \
	__uint(max_entries, MAP_ENTRY_AMOUNT); \
} map_name SEC(".maps");

/**
 * @brief License
 * 
 */
char LICENSE[] SEC("license") = "Dual BSD/GPL";

#ifndef MATCH_OK
/**
 * @brief BPF program decision if a match has been found
 * 
 */
#define MATCH_OK XDP_PASS
#endif

#ifndef MATCH_KO
/**
 * @brief BPF program decision if zero matches has been found
 * 
 */
#define MATCH_KO XDP_DROP
#endif

/**
 * @brief Ethernet filters
 * 
 */
MAP_CREATE(filter_eth, filter_key_eth_t)
/**
 * @brief IP filters
 * 
 */
MAP_CREATE(filter_ip, filter_key_ip_t)
/**
 * @brief IP6 filters
 * 
 */
MAP_CREATE(filter_ip6, filter_key_ip6_t)
/**
 * @brief TCP/UDP filters
 * 
 */
MAP_CREATE(filter_port, filter_key_port_t)
/**
 * @brief ICMP filters
 * 
 */
MAP_CREATE(filter_icmp, filter_key_icmp_t)

/**
 * @brief Check if there is a match,
 * if yes, it increments the match counter
 * 
 */
#define COMMON_LOOKUP_MATCH(map, k) \
    lookup = bpf_map_lookup_elem(&map, &k); \
    if (lookup) {\
        ((filter_value_t *) lookup)->count += 1;\
        return MATCH_OK; \
    }

/**
 * @brief Looking for MATCH_OK for filters that have
 * a source (src) and a destination (dst) field
 * 
 */
#define COMMON_CHECK(map, k, source, destination) \
    void *lookup; \
    \
	__builtin_memcpy(&k.src, source, sizeof(k.src)); \
	__builtin_memcpy(&k.dst, destination, sizeof(k.dst)); \
    COMMON_LOOKUP_MATCH(map, k) \
    \
    __builtin_memset(&k.src, 0, sizeof(k.src)); \
    COMMON_LOOKUP_MATCH(map, k) \
    \
    __builtin_memset(&k.dst, 0, sizeof(k.dst)); \
	__builtin_memcpy(&k.src, source, sizeof(k.src)); \
    COMMON_LOOKUP_MATCH(map, k)

/**
 * @brief Take Ethernet decision (layer 2)
 * 
 * @param nh 
 * @param data_end 
 * @param proto 
 * @return int
 */
static __always_inline int filter_l2(
    struct hdr_cursor *nh, void *data_end, int *proto)
{
    struct ethhdr *eth;

    *proto = parse_ethhdr(nh, data_end, &eth);
    if (*proto == -1)
        return XDP_DROP;

    *proto = bpf_htons(*proto);

    filter_key_eth_t k = {0};
    COMMON_CHECK(filter_eth, k, eth->h_source, eth->h_dest)

    return MATCH_KO;
}

/**
 * @brief Take IP decision
 * 
 * @param cursor 
 * @param data_end 
 * @param proto
 * @return int 
 */
static __always_inline int filter_ip_xdp_action(
    struct hdr_cursor *nh, void *data_end, int *proto)
{
    struct iphdr *ip;

    *proto = parse_iphdr(nh, data_end, &ip);
    if (*proto == -1)
        return XDP_DROP;

    filter_key_ip_t k = {0};

    __u32 src = bpf_ntohl(ip->addrs.saddr);
    __u32 dst = bpf_ntohl(ip->addrs.daddr);

    COMMON_CHECK(filter_ip, k, &src, &dst)

    return MATCH_KO;
}

/**
 * @brief Take IP6 decision
 * 
 * @param cursor 
 * @param data_end 
 * @param proto
 * @return int 
 */
static __always_inline int filter_ip6_xdp_action(
    struct hdr_cursor *nh, void *data_end, int *proto)
{
    struct ipv6hdr *ip6;

    *proto = parse_ip6hdr(nh, data_end, &ip6);
    if (*proto == -1)
        return XDP_DROP;
    
    filter_key_ip6_t k = {0};
    COMMON_CHECK(filter_ip6, k, &ip6->addrs.saddr, &ip6->addrs.daddr)

    return MATCH_KO;
}

/**
 * @brief Filter layer 3
 * 
 * @param nh 
 * @param data_end 
 * @param proto 
 * @return int 
 */
static __always_inline int filter_l3(
    struct hdr_cursor *nh, void *data_end, int *proto)
{
    switch (*proto)
    {
    case ETH_P_IP:
        return filter_ip_xdp_action(nh, data_end, proto);
    case ETH_P_IPV6:
        return filter_ip6_xdp_action(nh, data_end, proto);
    }

    *proto = -1;

    return XDP_PASS;
}

/**
 * @brief Take TCP/UDP decision
 * 
 * @param nh 
 * @param data_end 
 * @param proto 
 * @return int 
 */
static __always_inline int filter_udp_xdp_action(
    struct hdr_cursor *nh, void *data_end, __u8 proto)
{
    struct udphdr *udp = nh->pos;

	if (udp + 1 > (struct udphdr *) data_end)
		return XDP_DROP;

	nh->pos = udp + 1;
    
    filter_key_port_t k = {0};

    k.proto = proto;

    __u16 src = bpf_ntohs(udp->source);
    __u16 dst = bpf_ntohs(udp->dest);

    COMMON_CHECK(filter_port, k, &src, &dst)
    return MATCH_KO;
}

/**
 * @brief Tack ICMP decision
 * 
 * @param cursor 
 * @param data_end 
 * @return int 
 */
static __always_inline int filter_icmp_xdp_action(
    struct hdr_cursor *nh, void *data_end)
{
    int t;
    struct icmphdr_common *icmp;
    void *lookup;

    t = parse_icmphdr_common(nh, data_end, &icmp);
    if (t == -1)
        return XDP_DROP;
    
    filter_key_icmp_t k = {0};

    k.code = icmp->code;

    COMMON_LOOKUP_MATCH(filter_icmp, k)
    return MATCH_KO;
}

/**
 * @brief Filter layer 4
 * 
 * @param nh 
 * @param data_end 
 * @param proto 
 * @return int 
 */
static __always_inline int filter_l4(
    struct hdr_cursor *nh, void *data_end, int *proto)
{
    switch (*proto)
    {
    case IPPROTO_TCP:
    case IPPROTO_UDP:
        return filter_udp_xdp_action(nh, data_end, *proto);
    case IPPROTO_ICMP:
        return filter_icmp_xdp_action(nh, data_end);
    }

    *proto = -1;

    return XDP_PASS;
}

SEC("xdp")
/**
 * @brief XDP program
 * 
 * @param ctx 
 * @return int
 */
int xdp_packet_filter(struct xdp_md *ctx)
{
    int proto, result;

    void *data = (void *)(long) ctx->data;
    void *data_end = (void *)(long) ctx->data_end;
    struct hdr_cursor nh;

    nh.pos = data;

    // Layer 2
    result = filter_l2(&nh, data_end, &proto);
    if (proto == -1 || result == MATCH_OK)
        return result;
    
    // Layer 3
    result = filter_l3(&nh, data_end, &proto);
    if (proto == -1 || result == MATCH_OK)
        return result;
    
    // Layer 4
    result = filter_l4(&nh, data_end, &proto);

    return result;
}
