#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "../../argparse/argparse.h"
#include "../../common/bpf/bpf.h"
#include "../../filter/filter.h"
#include "../../command/command.h"
#include "../common/common.h"
#include "./icmp.h"

#include <xdp/libxdp.h>

/**
 * @brief BPF map name
 * 
 */
static const char map_name[] = "filter_icmp";

/**
 * @brief CLI usage
 * 
 */
static const char *const common_usages[] = {
    "add/del [options]",
    NULL,
};

/**
 * @brief Command icmp add parse
 * 
 * @param command 
 * @param argc 
 * @param argv 
 */
static void command_icmp_add_parse(command_icmp_add_t *command, int argc,
    const char *argv[])
{
    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Required options"),
        OPT_STRING('i', "ifname", &command->ifname, "Network interface name", NULL, 0, 0),
        OPT_INTEGER('c', "code", &command->code, "ICMP code", NULL, 0, 0),
        OPT_END(),
    };

    argparse_init(&argparse, options, common_usages, 0);
    argparse_describe(&argparse, NULL, NULL);
    argparse_parse(&argparse, argc, argv);
}

/**
 * @brief Command icmp add configure
 * 
 * @param argc 
 * @param argv 
 * @return command_icmp_add_t 
 */
static command_icmp_add_t command_icmp_add_configure(int argc, const char *argv[])
{
    command_icmp_add_t command = {0};

    command_icmp_add_parse(&command, argc, argv);

    return command;
}

/**
 * @brief Command icmp add check
 * 
 * @param command 
 * @return true 
 * @return false 
 */
static bool command_icmp_add_check(command_icmp_add_t *command)
{
    return command->ifname &&
        command->code >= 0;
}

/**
 * @brief Filling a BPF icmp map struct from CLI values
 * 
 * @param k 
 * @param cfg 
 * @return int 
 */
static int fill(filter_key_icmp_t *k, command_icmp_add_t *cfg)
{
    if (!cfg || !k)
        return EXIT_FAILURE;
    
    k->code = cfg->code;

    return EXIT_SUCCESS;
}

/**
 * @brief Command icmp pre process (add/del)
 * 
 * @param argc 
 * @param argv 
 * @param k 
 * @return int 
 */
static int command_pre_process(int argc, const char *argv[],
    filter_key_icmp_t *k)
{
    int err, map_fd;
    command_icmp_add_t cfg;
    
    cfg = command_icmp_add_configure(argc, argv);
    if (!command_icmp_add_check(&cfg))
        return -1;
    
    map_fd = common_bpf_pinned_map(cfg.ifname, map_name);
    if (map_fd < 0)
        return -1;

    err = fill(k, &cfg);
    if (err)
        return -1;
    
    return map_fd;
}

/**
 * @brief Print a single filter
 * 
 * @param k 
 * @param v 
 */
static void print_filter(filter_key_icmp_t *k, filter_value_t *v)
{
    if (!k || !v)
        return;

    printf("code %u, %llu matches\n", k->code, v->count);
}

int command_icmp_add_process(int argc, const char *argv[])
{
    COMMAND_COMMON_ADD(filter_key_icmp_t)
}

int command_icmp_remove_process(int argc, const char *argv[])
{
    COMMAND_COMMON_REMOVE(filter_key_icmp_t)
}

int command_icmp_filters_process(int argc, const char *argv[])
{
    COMMAND_COMMON_FILTERS(map_name, filter_key_icmp_t)
}
