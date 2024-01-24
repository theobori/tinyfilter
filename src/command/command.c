#include <stdlib.h>
#include <string.h>

#include "./command.h"
#include "./ip/ip.h"
#include "./ip6/ip6.h"
#include "./eth/eth.h"
#include "./port/port.h"
#include "./icmp/icmp.h"

/**
 * @brief Commands eth skeleton
 * 
 */
static command_entry_t commands_eth[] = {
    { "add", command_eth_add_process, NULL },
    { "del", command_eth_remove_process, NULL },
    { "filters", command_eth_filters_process, NULL },
    { "", NULL, NULL },
};

/**
 * @brief Commands ip skeleton
 * 
 */
static command_entry_t commands_ip[] = {
    { "add", command_ip_add_process, NULL },
    { "del", command_ip_remove_process, NULL },
    { "filters", command_ip_filters_process, NULL },
    { "", NULL, NULL },
};

/**
 * @brief Commands ip6 skeleton
 * 
 */
static command_entry_t commands_ip6[] = {
    { "add", command_ip6_add_process, NULL },
    { "del", command_ip6_remove_process, NULL },
    { "filters", command_ip6_filters_process, NULL },
    { "", NULL, NULL },
};

/**
 * @brief Commands port skeleton
 * 
 */
static command_entry_t commands_port[] = {
    { "add", command_port_add_process, NULL },
    { "del", command_port_remove_process, NULL },
    { "filters", command_port_filters_process, NULL },
    { "", NULL, NULL },
};

/**
 * @brief Commands icmp skeleton
 * 
 */
static command_entry_t commands_icmp[] = {
    { "add", command_icmp_add_process, NULL },
    { "del", command_icmp_remove_process, NULL },
    { "filters", command_icmp_filters_process, NULL },
    { "", NULL, NULL },
};

/**
 * @brief Commands skeleton
 * 
 */
static command_entry_t commands[] = {
    { "load", command_load_process, NULL },
    { "unload", command_unload_process, NULL },
    { "eth", NULL, commands_eth },
    { "ip", NULL, commands_ip },
    { "ip6", NULL, commands_ip6 },
    { "port", NULL, commands_port },
    { "icmp", NULL, commands_icmp },
    { "", NULL, NULL },
};

command_entry_t *get_commands()
{
    return commands;
}

/**
 * @brief Print help
 * 
 * @param commands 
 * @return int 
 */
static int print_commands_help(command_entry_t *commands)
{
    printf("You can use the following commands:\n");

    for (unsigned int i = 0; commands[i].process || commands[i].next; i++) {
        printf("- %s\n", commands[i].name);
    }

    return EXIT_SUCCESS;
}

int command_process(int argc, const char *command_start[], 
    command_entry_t *commands)
{
    int err;
    command_entry_t command;
    const char *name;

    if (!command_start || !commands || argc < 1)
        return EXIT_FAILURE;

    name = command_start[0];

    for (unsigned int i = 0; commands[i].process || commands[i].next; i++) {
        command = commands[i];

        err = strcmp(name, "help");
        if (!err)
            return print_commands_help(commands);

        err = strcmp(name, (const char *) command.name);
        if (err)
            continue;
        
        if (command.next)
            return command_process(argc - 1, command_start + 1, command.next);
        
        if (!command.process)
            continue;

        err = (command.process)(argc, command_start);
        if (err)
            return EXIT_FAILURE;
        
        return EXIT_SUCCESS;
    }    

    return EXIT_FAILURE;
}
