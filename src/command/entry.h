#ifndef __COMMAND_ENTRY_H__
#define __COMMAND_ENTRY_H__

#include <stdbool.h>

#define COMMAND_NAME_MAX 16

typedef struct command_entry_s {
    char name[COMMAND_NAME_MAX];
    int (*process)(int argc, const char *argv[]);
    struct command_entry_s *next;
} command_entry_t;

#endif