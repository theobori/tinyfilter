#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../argparse/argparse.h"
#include "./ip.h"
#include "../../common/bpf/bpf.h"
#include "../../filter/filter.h"
#include "../../command/command.h"

#include <xdp/libxdp.h>

/**
 * @brief CLI usage
 * 
 */
static const char *const ip_common_usages[] = {
    "add/del [options]",
    NULL,
};

/**
 * @brief BPF map name
 * 
 */
static const char *ip_map = "filter_ip";

/**
 * @brief Common ip CLI parse
 * 
 * @param command 
 * @param argc 
 * @param argv 
 */
static void command_ip_common_parse(command_ip_common_t *command, int argc,
    const char *argv[])
{
    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Required options"),
        OPT_STRING('i', "ifname", &command->ifname, "Network interface name", NULL, 0, 0),
        OPT_GROUP("Optionnals options"),
        OPT_STRING('s', "src", &command->src, "Source IP address", NULL, 0, 0),
        OPT_STRING('d', "dst", &command->dst, "Destination IP address", NULL, 0, 0),
        OPT_END(),
    };

    argparse_init(&argparse, options, ip_common_usages, 0);
    argparse_describe(&argparse, NULL, NULL);
    argparse_parse(&argparse, argc, argv);
}

/**
 * @brief Common ip configure
 * 
 * @param argc 
 * @param argv 
 * @return command_ip_common_t 
 */
static command_ip_common_t command_ip_common_configure(int argc, const char *argv[])
{
    command_ip_common_t command = {0};

    command_ip_common_parse(&command, argc, argv);

    return command;
}

/**
 * @brief Common ip check
 * 
 * @param command 
 * @return true 
 * @return false 
 */
static bool command_ip_common_check(command_ip_common_t *command)
{
    return command->ifname &&
        (command->dst || command->src);
}

/**
 * @brief Filling a BPF ip map struct from CLI values
 * 
 * @param k 
 * @param cfg 
 * @return int 
 */
static int ip_fill(filter_key_ip_t *k, command_ip_common_t *cfg)
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
 * @brief Common ip pre process
 * 
 * @param map_fd 
 * @param k 
 * @param cfg 
 * @return int 
 */
static int command_ip_common_pre_process(int *map_fd, filter_key_ip_t *k,
    command_ip_common_t *cfg)
{
    int err;

    if (!command_ip_common_check(&cfg))
        return EXIT_FAILURE;
    
    *map_fd = common_bpf_pinned_map(cfg->ifname, ip_map);
    if (*map_fd < 0)
        return EXIT_FAILURE;

    err = ip_fill(k, cfg);
    printf("%d\n", err);
    if (err)
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

int command_ip_add_process(int argc, const char *argv[])
{
    int err, map_fd;
    filter_key_ip_t k;
    command_ip_common_t cfg = command_ip_common_configure(argc, argv);

    err = command_ip_common_pre_process(&map_fd, &k, &cfg);
    if (err)
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
    command_ip_common_t cfg = command_ip_common_configure(argc, argv);

    err = command_ip_common_pre_process(&map_fd, &k, &cfg);
    if (err)
        return EXIT_FAILURE;
    
    err = bpf_map_delete_elem(map_fd, &k);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/**
 * @brief CLI usage
 * 
 */
static const char *const ip_show_usages[] = {
    "show [options]",
    NULL,
};

/**
 * @brief ip show parse
 * 
 * @param command 
 * @param argc 
 * @param argv 
 */
static void command_ip_show_parse(command_ip_show_t *command, int argc,
    const char *argv[])
{
    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Required options"),
        OPT_STRING('i', "ifname", &command->ifname, "Network interface name", NULL, 0, 0),
        OPT_END(),
    };

    argparse_init(&argparse, options, ip_show_usages, 0);
    argparse_describe(&argparse, NULL, NULL);
    argparse_parse(&argparse, argc, argv);
}

/**
 * @brief ip show configure
 * 
 * @param argc 
 * @param argv 
 * @return command_ip_show_t 
 */
static command_ip_show_t command_ip_show_configure(int argc, const char *argv[])
{
    command_ip_show_t command = {0};

    command_ip_show_parse(&command, argc, argv);

    return command;
}

/**
 * @brief ip show check
 * 
 * @param command 
 * @return true 
 * @return false 
 */
static bool command_ip_show_check(command_ip_show_t *command)
{
    return command->ifname;
}

int command_ip_show_process(int argc, const char *argv[])
{
    int err;
    command_ip_show_t cfg = command_ip_show_configure(argc, argv);

    if (!command_ip_show_check(&cfg))
        return EXIT_FAILURE;

    // TODO

    return EXIT_SUCCESS;
}
