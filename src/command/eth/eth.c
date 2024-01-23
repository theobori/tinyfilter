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
static const char map_name[] = "filter_ethernet";

/**
 * @brief Ethernet null addr used for comparaison
 * 
 */
static const unsigned char eth_addr_null[ETH_ALEN] = {0};

/**
 * @brief Filling a BPF eth map struct from CLI values
 * 
 * @param k 
 * @param cfg 
 * @return int 
 */
static int eth_fill(filter_key_eth_t *k, command_common_add_t *cfg)
{
    int err;

    if (!cfg || !k)
        return EXIT_FAILURE;
    
    err = eth_raw_value(cfg->src, k->src);
    err &= eth_raw_value(cfg->dst, k->dst);

    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/**
 * @brief Command eth pre process (add/del)
 * 
 * @param argc 
 * @param argv 
 * @param k 
 * @return int 
 */
static int command_eth_pre_process(int argc, const char *argv[],
    filter_key_eth_t *k)
{
    int err, map_fd;
    command_common_add_t cfg;
    
    cfg = command_common_add_configure(argc, argv);
    if (!command_common_add_check(&cfg))
        return -1;
    
    map_fd = common_bpf_pinned_map(cfg.ifname, map_name);
    if (map_fd < 0)
        return -1;

    err = eth_fill(k, &cfg);
    if (err)
        return -1;

    return map_fd;
}

int command_eth_add_process(int argc, const char *argv[])
{
    int err, map_fd;
    filter_key_eth_t k = {0};

    map_fd = command_eth_pre_process(argc, argv, &k);
    if (map_fd < 0)
        return EXIT_FAILURE;
    
    filter_value_t v = {0};

    err = bpf_map_update_elem(map_fd, &k, &v, BPF_ANY);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int command_eth_remove_process(int argc, const char *argv[])
{
    int err, map_fd;
    filter_key_eth_t k = {0};

    map_fd = command_eth_pre_process(argc, argv, &k);
    if (map_fd < 0)
        return EXIT_FAILURE;
    
    err = bpf_map_delete_elem(map_fd, &k);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}