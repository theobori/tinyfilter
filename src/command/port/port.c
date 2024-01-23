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
static const char map_name[] = "filter_port";

/**
 * @brief Filling a BPF port map struct from CLI values
 * 
 * @param k 
 * @param cfg 
 * @return int 
 */
static int port_fill(filter_key_port_t *k, command_common_add_t *cfg)
{
    if (!cfg || !k)
        return EXIT_FAILURE;
    
    k->dst = port_raw_value(cfg->dst);
    k->src = port_raw_value(cfg->src);
    k->proto = IPPROTO_TCP;

    if (!k->src && !k->dst)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/**
 * @brief Command port pre process (add/del)
 * 
 * @param argc 
 * @param argv 
 * @param k 
 * @return int 
 */
static int command_port_pre_process(int argc, const char *argv[],
    filter_key_port_t *k)
{
    int err, map_fd;
    command_common_add_t cfg;
    
    cfg = command_common_add_configure(argc, argv);
    if (!command_common_add_check(&cfg))
        return -1;
    
    map_fd = common_bpf_pinned_map(cfg.ifname, map_name);
    if (map_fd < 0)
        return -1;

    err = port_fill(k, &cfg);
    if (err)
        return -1;
    
    return map_fd;
}

int command_port_add_process(int argc, const char *argv[])
{
    int err, map_fd;
    filter_key_port_t k = {0};

    map_fd = command_port_pre_process(argc, argv, &k);
    if (map_fd < 0)
        return EXIT_FAILURE;

    filter_value_t v = {0};

    err = bpf_map_update_elem(map_fd, &k, &v, BPF_ANY);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int command_port_remove_process(int argc, const char *argv[])
{
    int err, map_fd;
    filter_key_port_t k = {0};

    map_fd = command_port_pre_process(argc, argv, &k);
    if (map_fd < 0)
        return EXIT_FAILURE;
    
    err = bpf_map_delete_elem(map_fd, &k);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

static void print_filter(filter_key_port_t *k, filter_value_t *v)
{
    if (k->src)
        printf("Protocol %hu, from %hu", k->proto, k->src);

    if (k->dst) {
        if (k->src)
            printf(" to ");
        else
            printf("Protocol %hu, To ", k->proto);

        printf("%hu", k->dst);
    }

    printf(", %llu matches\n", v->count);
}

int command_port_filters_process(int argc, const char *argv[])
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

        print_filter((filter_key_port_t *) &key, (filter_value_t *) &value);

        prev_key = key;
    }

    return EXIT_SUCCESS;
}
