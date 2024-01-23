#ifndef __COMMAND_LOAD_H__
#define __COMMAND_LOAD_H__

#include <xdp/libxdp.h>

/**
 * @brief Command load CLI arguments representation
 * 
 */
typedef struct command_load_s {
    char *ifname;
    char *bpf_object_file;
    enum xdp_attach_mode xdp_mode;
} command_load_t;

/**
 * @brief Command load process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_load_process(int argc, const char *argv[]);

#endif
