#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../argparse/argparse.h"
#include "../../common/xdp/xdp.h"
#include "./unload.h"

#include <xdp/libxdp.h>

/**
 * @brief CLI usage
 *
 */
static const char *const usages[] = {
    "unload [options]",
    NULL,
};

/**
 * @brief Command description
 *
 */
static const char *unload_description =
    "\nUnload a BPF program and unattach it from a network interface.";

/**
 * @brief Command unload parse
 *
 * @param command
 * @param argc
 * @param argv
 */
static void command_unload_parse(command_unload_t *command, int argc,
                                 const char *argv[]) {
  char *xdp_mode_name = NULL;

  struct argparse argparse;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_GROUP("Required options"),
      OPT_STRING('i', "ifname", &command->ifname, "Network interface name",
                 NULL, 0, 0),
      OPT_GROUP("Optional options"),
      OPT_INTEGER('I', "id", &command->prog_id, "BPF program ID", NULL, 0, 0),
      OPT_STRING('m', "xdp-mode", &xdp_mode_name, "XDP mode, by default skb",
                 NULL, 0, 0),
      OPT_END(),
  };

  argparse_init(&argparse, options, usages, 0);
  argparse_describe(&argparse, unload_description, NULL);
  argparse_parse(&argparse, argc, argv);

  command->xdp_mode = common_xdp_mode_from_name(xdp_mode_name);
}

/**
 * @brief Command unload configure
 *
 * @param argc
 * @param argv
 * @return command_unload_t
 */
static command_unload_t command_unload_configure(int argc, const char *argv[]) {
  command_unload_t command = {0};

  command_unload_parse(&command, argc, argv);

  return command;
}

/**
 * @brief COmmand unload check
 *
 * @param command
 * @return true
 * @return false
 */

static bool command_unload_check(command_unload_t *command) {
  return command->ifname && command->xdp_mode > 0 && command->xdp_mode < 4;
}

int command_unload_process(int argc, const char *argv[]) {
  int err;
  command_unload_t cfg = command_unload_configure(argc, argv);

  if (!command_unload_check(&cfg))
    return EXIT_FAILURE;

  err = common_xdp_unload(cfg.prog_id, cfg.ifname, cfg.xdp_mode);
  if (err)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
