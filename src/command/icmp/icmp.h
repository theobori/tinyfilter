#ifndef __COMMAND_ICMP_H__
#define __COMMAND_ICMP_H__

#include "../../filter/filter.h"

/**
 * @brief Command icmp add CLI arguments representation
 * 
 */
typedef struct command_icmp_add_s {
    char *ifname;
    unsigned char code;
} command_icmp_add_t;

/**
 * @brief Command icmp add process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_icmp_add_process(int argc, const char *argv[]);

/**
 * @brief Command icmp remove process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_icmp_remove_process(int argc, const char *argv[]);

/**
 * @brief Command icmp show process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_icmp_filters_process(int argc, const char *argv[]);

#endif
