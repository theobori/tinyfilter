#ifndef __COMMAND_IP_H__
#define __COMMAND_IP_H__

/**
 * @brief Command ip common cli arguments
 * 
 */
typedef struct command_ip_common_s {
    char *ifname;
    char *src;
    char *dst;
} command_ip_common_t;

/**
 * @brief command ip show cli arguments
 * 
 */
typedef struct command_ip_show_s {
    char *ifname;
} command_ip_show_t;

int command_ip_add_process(int argc, const char *argv[]);
int command_ip_remove_process(int argc, const char *argv[]);
int command_ip_show_process(int argc, const char *argv[]);

#endif
