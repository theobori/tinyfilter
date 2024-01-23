#ifndef __COMMAND_ip66_H__
#define __COMMAND_ip66_H__

/**
 * @brief Command ip6 add process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_ip6_add_process(int argc, const char *argv[]);

/**
 * @brief Command ip6 remove process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_ip6_remove_process(int argc, const char *argv[]);

/**
 * @brief Command ip6 show process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_ip6_filters_process(int argc, const char *argv[]);

#endif
