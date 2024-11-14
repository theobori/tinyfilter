#ifndef __COMMAND_PORT_H__
#define __COMMAND_PORT_H__

/**
 * @brief Command port add CLI arguments representation
 *
 */
typedef struct command_port_add_s {
  char *ifname;
  char *src;
  char *dst;
  char *proto;
} command_port_add_t;

/**
 * @brief Command port add process
 *
 * @param argc
 * @param argv
 * @return int
 */
int command_port_add_process(int argc, const char *argv[]);

/**
 * @brief Command port remove process
 *
 * @param argc
 * @param argv
 * @return int
 */
int command_port_remove_process(int argc, const char *argv[]);

/**
 * @brief Command port show process
 *
 * @param argc
 * @param argv
 * @return int
 */
int command_port_filters_process(int argc, const char *argv[]);

#endif
