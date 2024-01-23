#ifndef __COMMAND_COMMON_H__
#define __COMMAND_COMMON_H__

#include <stdbool.h>

/**
 * @brief Command common cli arguments
 * 
 */
typedef struct command_common_add_s {
    char *ifname;
    char *src;
    char *dst;
} command_common_add_t;

/**
 * @brief Command filters cli arguments
 * 
 */
typedef struct command_common_filters_s {
    char *ifname;
} command_common_filters_t;

/**
 * @brief Command common add process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_common_add_process(int argc, const char *argv[]);

/**
 * @brief Command common remove process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int command_common_remove_process(int argc, const char *argv[]);

/**
 * @brief Command common show process
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */

/**
 * @brief Command command add parse
 * 
 * @param command 
 * @param argc 
 * @param argv 
 */
void command_common_add_parse(command_common_add_t *command, int argc,
    const char *argv[]);

/**
 * @brief Command common add configure
 * 
 * @param argc 
 * @param argv 
 * @return command_common_add_t 
 */
command_common_add_t command_common_add_configure(int argc, const char *argv[]);

/**
 * @brief Command common add check
 * 
 * @param command 
 * @return true 
 * @return false 
 */
bool command_common_add_check(command_common_add_t *command);

/**
 * @brief Command common filters parse
 * 
 * @param command 
 * @param argc 
 * @param argv 
 */
void command_common_filters_parse(command_common_filters_t *command, int argc,
    const char *argv[]);

/**
 * @brief Command common filters configure
 * 
 * @param argc 
 * @param argv 
 * @return command_common_filters_t 
 */
command_common_filters_t command_common_filters_configure(int argc,
    const char *argv[]);

/**
 * @brief Command common filters check
 * 
 * @param command 
 * @return true 
 * @return false 
 */
bool command_common_filters_check(command_common_filters_t *command);

#endif
