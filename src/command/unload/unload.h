#ifndef __COMMAND_UNLOAD_H__
#define __COMMAND_UNLOAD_H__

typedef struct command_unload_s {
    char *ifname;
    unsigned int prog_id;
    enum xdp_attach_mode xdp_mode;
} command_unload_t;

int command_unload_process(int argc, const char *argv[]);

#endif
