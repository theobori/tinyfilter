#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../argparse/argparse.h"
#include "../../command/command.h"
#include "../../common/bpf/bpf.h"
#include "../../filter/filter.h"
#include "../common/common.h"
#include "./port.h"

#include <xdp/libxdp.h>

/**
 * @brief BPF map name
 *
 */
static const char map_name[] = "filter_port";

/**
 * @brief CLI usage
 *
 */
static const char *const common_usages[] = {
    "add/del [options]",
    NULL,
};

/**
 * @brief Command port add parse
 *
 * @param command
 * @param argc
 * @param argv
 */
static void command_port_add_parse(command_port_add_t *command, int argc,
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
      OPT_STRING('p', "proto", &command->proto, "Layer 4 protocol", NULL, 0, 0),
      OPT_END(),
  };

  if (!command->proto)
    command->proto = "tcp";

  argparse_init(&argparse, options, common_usages, 0);
  argparse_describe(&argparse, NULL, NULL);
  argparse_parse(&argparse, argc, argv);
}

/**
 * @brief Command port add configure
 *
 * @param argc
 * @param argv
 * @return command_port_add_t
 */
static command_port_add_t command_port_add_configure(int argc,
                                                     const char *argv[]) {
  command_port_add_t command = {0};

  command_port_add_parse(&command, argc, argv);

  return command;
}

/**
 * @brief Command port add check
 *
 * @param command
 * @return true
 * @return false
 */
static bool command_port_add_check(command_port_add_t *command) {
  return command->ifname && (command->dst || command->src) && command->proto;
}

/**
 * @brief Filling a BPF port map struct from CLI values
 *
 * @param k
 * @param cfg
 * @return int
 */
static int fill(filter_key_port_t *k, command_port_add_t *cfg) {
  int err;

  if (!cfg || !k)
    return EXIT_FAILURE;

  err = l4_proto_resolve(cfg->proto, &k->proto, VALUE);
  if (err)
    return EXIT_FAILURE;

  if (!k->proto)
    return EXIT_FAILURE;

  k->dst = port_raw_value(cfg->dst);
  k->src = port_raw_value(cfg->src);

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
static int command_pre_process(int argc, const char *argv[],
                               filter_key_port_t *k) {
  int err, map_fd;
  command_port_add_t cfg;

  cfg = command_port_add_configure(argc, argv);
  if (!command_port_add_check(&cfg))
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
 * @brief Print a singl filter
 *
 * @param k
 * @param v
 */
static void print_filter(filter_key_port_t *k, filter_value_t *v) {
  int err;
  char name[10];

  err = l4_proto_resolve(name, &k->proto, NAME);
  if (err)
    return;

  if (k->src)
    printf("Protocol %s, from %hu", name, k->src);

  if (k->dst) {
    if (k->src)
      printf(" to ");
    else
      printf("Protocol %s, To ", name);

    printf("%hu", k->dst);
  }

  printf(", %llu matches\n", v->count);
}

int command_port_add_process(int argc, const char *argv[]) {
  COMMAND_COMMON_ADD(filter_key_port_t)
}

int command_port_remove_process(int argc, const char *argv[]) {
  COMMAND_COMMON_REMOVE(filter_key_port_t)
}

int command_port_filters_process(int argc, const char *argv[]) {
  COMMAND_COMMON_FILTERS(map_name, filter_key_port_t)
}
