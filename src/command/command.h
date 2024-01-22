#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "./load/load.h"
#include "./unload/unload.h"
#include "./help/help.h"
#include "./ip/ip.h"

#include "../common/common.h"
#include "./entry.h"

#define PROGRAM_NAME "tinyfilter"

int command_process(int argc, const char *command_start[], 
    command_entry_t *commands);
command_entry_t *get_commands();

#endif
