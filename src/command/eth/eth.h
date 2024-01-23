#ifndef __COMMAND_ETH_H__
#define __COMMAND_ETH_H__

/**
 * @brief Command eth add process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_eth_add_process(int argc, const char *argv[]);

/**
 * @brief Command eth remove process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_eth_remove_process(int argc, const char *argv[]);

/**
 * @brief Command eth filters process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_eth_filters_process(int argc, const char *argv[]);

#endif
