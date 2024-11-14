#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "./ip/ip.h"
#include "./load/load.h"
#include "./unload/unload.h"

#include "../common/common.h"
#include "./entry.h"

/**
 * @brief Program name
 *
 */
#define PROGRAM_NAME "tinyfilter"

/**
 * @brief Command process
 *
 * @param argc
 * @param command_start
 * @param commands
 * @return int
 */
int command_process(int argc, const char *command_start[],
                    command_entry_t *commands);

/**
 * @brief Get the commands object
 *
 * @return command_entry_t*
 */
command_entry_t *get_commands();

#endif
