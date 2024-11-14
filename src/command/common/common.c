#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../argparse/argparse.h"
#include "./common.h"

#include <xdp/libxdp.h>

/**
 * @brief CLI usage
 *
 */
static const char *const common_usages[] = {
    "add/del [options]",
    NULL,
};

void command_common_add_parse(command_common_add_t *command, int argc,
                              const char *argv[]) {
  struct argparse argparse;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_GROUP("Required options"),
      OPT_STRING('i', "ifname", &command->ifname, "Network interface name",
                 NULL, 0, 0),
      OPT_GROUP("Optionnals options"),
      OPT_STRING('s', "src", &command->src, "Source value", NULL, 0, 0),
      OPT_STRING('d', "dst", &command->dst, "Destination value", NULL, 0, 0),
      OPT_END(),
  };

  argparse_init(&argparse, options, common_usages, 0);
  argparse_describe(&argparse, NULL, NULL);
  argparse_parse(&argparse, argc, argv);
}

command_common_add_t command_common_add_configure(int argc,
                                                  const char *argv[]) {
  command_common_add_t command = {0};

  command_common_add_parse(&command, argc, argv);

  return command;
}

bool command_common_add_check(command_common_add_t *command) {
  return command->ifname && (command->dst || command->src);
}

/**
 * @brief CLI usage
 *
 */
static const char *const eth_show_usages[] = {
    "filters [options]",
    NULL,
};

void command_common_filters_parse(command_common_filters_t *command, int argc,
                                  const char *argv[]) {
  struct argparse argparse;
  struct argparse_option options[] = {
      OPT_HELP(),
      OPT_GROUP("Required options"),
      OPT_STRING('i', "ifname", &command->ifname, "Network interface name",
                 NULL, 0, 0),
      OPT_END(),
  };

  argparse_init(&argparse, options, eth_show_usages, 0);
  argparse_describe(&argparse, NULL, NULL);
  argparse_parse(&argparse, argc, argv);
}

command_common_filters_t command_common_filters_configure(int argc,
                                                          const char *argv[]) {
  command_common_filters_t command = {0};

  command_common_filters_parse(&command, argc, argv);

  return command;
}

bool command_common_filters_check(command_common_filters_t *command) {
  return command->ifname;
}
