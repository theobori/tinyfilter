#ifndef __COMMAND_IP_H__
#define __COMMAND_IP_H__

/**
 * @brief Command ip add process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_ip_add_process(int argc, const char *argv[]);

/**
 * @brief Command ip remove process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_ip_remove_process(int argc, const char *argv[]);

/**
 * @brief Command ip show process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_ip_filters_process(int argc, const char *argv[]);

#endif
