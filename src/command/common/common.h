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

/**
 * @brief Generic command filter routine
 * 
 */
#define COMMAND_COMMON_FILTERS(map_name, key_type) \
    int err, map_fd;\
    \
    key_type key, prev_key;\
    filter_value_t value;\
\
    command_common_filters_t cfg = command_common_filters_configure(argc, argv);\
    if (!command_common_filters_check(&cfg))\
        return EXIT_FAILURE;\
\
    map_fd = common_bpf_pinned_map(cfg.ifname, map_name);\
    if (map_fd < 0)\
        return EXIT_FAILURE;\
    \
    while (!bpf_map_get_next_key(map_fd, &prev_key, &key)) {\
        err = bpf_map_lookup_elem(map_fd, &key, &value);\
        if (err)\
            continue;\
\
        print_filter(&key, &value);\
\
        prev_key = key;\
    }\
\
    return EXIT_SUCCESS;

/**
 * @brief Generic command init routine
 * 
 */
#define __COMMAND_COMMON_PRE(key_type) \
    int err, map_fd; \
    key_type k = {0}; \
\
    map_fd = command_pre_process(argc, argv, &k); \
    if (map_fd < 0) \
        return EXIT_FAILURE;

/**
 * @brief Generic command add routine
 * 
 */
#define COMMAND_COMMON_ADD(key_type) \
    __COMMAND_COMMON_PRE(key_type) \
\
    filter_value_t v = {0}; \
\
    err = bpf_map_update_elem(map_fd, &k, &v, BPF_ANY); \
    if (err) \
        return EXIT_FAILURE; \
\
    return EXIT_SUCCESS;

/**
 * @brief Generic command del routine
 * 
 */
#define COMMAND_COMMON_REMOVE(key_type) \
    __COMMAND_COMMON_PRE(key_type) \
    err = bpf_map_delete_elem(map_fd, &k); \
    if (err) \
        return EXIT_FAILURE; \
\
    return EXIT_SUCCESS;

#endif
