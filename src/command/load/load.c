#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../argparse/argparse.h"
#include "../../common/xdp/xdp.h"
#include "./load.h"

#include <xdp/libxdp.h>

/**
 * @brief CLI usage
 * 
 */
static const char *const usages[] = {
    "load [options]",
    NULL,
};

static const char *load_description = "\nLoads a BPF program and attach it to a network interface.";
static const char* bpf_object_file_default = "./src/tinyfilter.bpf.o";

static void command_load_parse(command_load_t *command, int argc,
    const char *argv[])
{
    char* xdp_mode_name = NULL;

    struct argparse argparse;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Required options"),
        OPT_STRING('i', "ifname", &command->ifname, "Network interface name", NULL, 0, 0),
        OPT_GROUP("Optional options"),
        OPT_STRING('b', "bpf-file", &command->bpf_object_file, "BPF object file path", NULL, 0, 0),
        OPT_STRING('m', "xdp-mode", &xdp_mode_name, "XDP mode, by default skb", NULL, 0, 0),
        OPT_END(),
    };

    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, load_description, NULL);
    argparse_parse(&argparse, argc, argv);

    if (!command->bpf_object_file)
        command->bpf_object_file = (char *) bpf_object_file_default;

    command->xdp_mode = common_xdp_mode_from_name(xdp_mode_name);
}

static command_load_t command_load_configure(int argc, const char *argv[])
{
    command_load_t command = {0};

    command_load_parse(&command, argc, argv);

    return command;
}

static bool command_load_check(command_load_t *command)
{
    return command->ifname &&
        command->xdp_mode > 0 &&
        command->xdp_mode < 4;
}

int command_load_process(int argc, const char *argv[])
{
    int err;

    command_load_t cfg = command_load_configure(argc, argv);

    if (!command_load_check(&cfg))
        return EXIT_FAILURE;
    
    err = common_xdp_load(cfg.ifname, cfg.bpf_object_file, cfg.xdp_mode);
    if (err)
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}
