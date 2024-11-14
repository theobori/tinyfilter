#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bpf/libbpf.h>
#include <xdp/libxdp.h>

#include "./command/command.h"

/**
 * @brief CLI minimal arguments amount
 *
 */
#define ARG_COUNT 2

/**
 * @brief Main function
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, const char *argv[]) {
  int err;

  if (argc < ARG_COUNT) {
    fprintf(stderr, "Check the help command.\n");
    return EXIT_FAILURE;
  }

  err = command_process(argc - 1, argv + 1, get_commands());
  if (err)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
