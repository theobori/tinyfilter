#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../command/command.h"
#include "../../common/bpf/bpf.h"
#include "../../filter/filter.h"
#include "../common/common.h"

#include <xdp/libxdp.h>

/**
 * @brief Null comparator
 *
 */
static const unsigned char ip6_null[16] = {0};

/**
 * @brief BPF map name
 *
 */
static const char map_name[] = "filter_ip6";

/**
 * @brief Filling a BPF ip6 map struct from CLI values
 *
 * @param k
 * @param cfg
 * @return int
 */
static int fill(filter_key_ip6_t *k, command_common_add_t *cfg) {
  int err;

  if (!cfg || !k)
    return EXIT_FAILURE;

  err = ip_raw_value(AF_INET6, cfg->src, k->src, sizeof(k->src));
  err &= ip_raw_value(AF_INET6, cfg->dst, k->dst, sizeof(k->dst));

  if (err)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

/**
 * @brief Command ip6 pre process (add/del)
 *
 * @param argc
 * @param argv
 * @param k
 * @return int
 */
static int command_pre_process(int argc, const char *argv[],
                               filter_key_ip6_t *k) {
  int err, map_fd;
  command_common_add_t cfg;

  cfg = command_common_add_configure(argc, argv);
  if (!command_common_add_check(&cfg))
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
static void print_filter(filter_key_ip6_t *k, filter_value_t *v) {
  char str[INET6_ADDRSTRLEN];
  bool is_src = memcmp(k->src, ip6_null, sizeof(k->src));

  if (is_src) {
    ip_string_value(AF_INET6, (char *)&k->src, str);
    printf("From %s", str);
  }

  if (memcmp(k->dst, ip6_null, sizeof(k->dst))) {
    ip_string_value(AF_INET6, (char *)&k->dst, str);

    if (is_src)
      printf(" to ");
    else
      printf("To ");

    printf("%s", str);
  }

  printf(", %llu matches\n", v->count);
}

int command_ip6_add_process(int argc, const char *argv[]) {
  COMMAND_COMMON_ADD(filter_key_ip6_t)
}

int command_ip6_remove_process(int argc, const char *argv[]) {
  COMMAND_COMMON_REMOVE(filter_key_ip6_t)
}

int command_ip6_filters_process(int argc, const char *argv[]) {
  COMMAND_COMMON_FILTERS(map_name, filter_key_ip6_t)
}
