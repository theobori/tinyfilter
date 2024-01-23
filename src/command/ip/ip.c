#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "../../argparse/argparse.h"
#include "../../common/bpf/bpf.h"
#include "../../filter/filter.h"
#include "../../command/command.h"
#include "../common/common.h"

#include <xdp/libxdp.h>


/**
 * @brief BPF map name
 * 
 */
static const char map_name[] = "filter_ip";

/**
 * @brief Filling a BPF ip map struct from CLI values
 * 
 * @param k 
 * @param cfg 
 * @return int 
 */
static int ip_fill(filter_key_ip_t *k, command_common_add_t *cfg)
{
    if (!cfg || !k)
        return EXIT_FAILURE;
    
    k->dst = ip4_raw_value(cfg->dst);
    k->src = ip4_raw_value(cfg->src);

    if (!k->src && !k->dst)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/**
 * @brief Command ip pre process (add/del)
 * 
 * @param argc 
 * @param argv 
 * @param k 
 * @return int 
 */
static int command_ip_pre_process(int argc, const char *argv[],
    filter_key_ip_t *k)
{
    int err, map_fd;
    command_common_add_t cfg;
    
    cfg = command_common_add_configure(argc, argv);
    if (!command_common_add_check(&cfg))
        return -1;
    
    map_fd = common_bpf_pinned_map(cfg.ifname, map_name);
    if (map_fd < 0)
        return -1;

    err = ip_fill(k, &cfg);
    if (err)
        return -1;
    
    return map_fd;
}

int command_ip_add_process(int argc, const char *argv[])
{
    int err, map_fd;
    filter_key_ip_t k;

    map_fd = command_ip_pre_process(argc, argv, &k);
    if (map_fd < 0)
        return EXIT_FAILURE;

    filter_value_t v = {0};

    err = bpf_map_update_elem(map_fd, &k, &v, BPF_ANY);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int command_ip_remove_process(int argc, const char *argv[])
{
    int err, map_fd;
    filter_key_ip_t k;

    map_fd = command_ip_pre_process(argc, argv, &k);
    if (map_fd < 0)
        return EXIT_FAILURE;
    
    err = bpf_map_delete_elem(map_fd, &k);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

static void print_filter(filter_key_ip_t *k, filter_value_t *v)
{
    unsigned int ip_host_order;
    char str[INET6_ADDRSTRLEN];

    if (k->src) {
        ip_host_order = ntohl(k->src);
        ip_string_value(AF_INET, (char *) &ip_host_order, str);
        printf("From %s", str);
    }

    if (k->dst) {
        ip_host_order = ntohl(k->dst);
        ip_string_value(AF_INET, (char *) &ip_host_order, str);

        if (k->src)
            printf(" to ");
        else
            printf("To ");

        printf("%s", str);
    }

    printf(", %llu matches\n", v->count);
}

int command_ip_filters_process(int argc, const char *argv[])
{
    int err, map_fd;
    unsigned long long key, prev_key, value;

    command_common_filters_t cfg = command_common_filters_configure(argc, argv);
    if (!command_common_filters_check(&cfg))
        return EXIT_FAILURE;

    map_fd = common_bpf_pinned_map(cfg.ifname, map_name);
    if (map_fd < 0)
        return EXIT_FAILURE;
    
    while (!bpf_map_get_next_key(map_fd, &prev_key, &key)) {
        err = bpf_map_lookup_elem(map_fd, &key, &value);

        if (err < 0)
            continue;

        print_filter((filter_key_ip_t *) &key, (filter_value_t *) &value);

        prev_key = key;
    }

    return EXIT_SUCCESS;
}
