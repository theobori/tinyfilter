#ifndef __COMMON_H__
#define __COMMON_H__

#include <linux/types.h>

/**
 * @brief Ethernet string representation length
 *
 */
#define ETH_STR_LEN 18

/**
 * @brief Compute static array length
 *
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

/**
 * @brief l4 proto string resolve entry
 *
 */
typedef struct l4_proto_string_s {
  char name[10];
  unsigned char value;
} l4_proto_string_t;

/**
 * @brief Layer proto resolve choice
 *
 */
enum l4_proto_resolve_choice { NAME = 0, VALUE };

/**
 * @brief Resolve layer 4 name or value
 *
 * @param name
 * @param value
 * @return int
 */
int l4_proto_resolve(char *name, unsigned char *value,
                     enum l4_proto_resolve_choice);

/**
 * @brief Convert IP address from string to its raw value
 *
 * @param af
 * @param src
 * @param dest
 * @param size
 * @return int
 */
int ip_raw_value(int af, const char *src, void *dest, unsigned int size);

/**
 * @brief Return the IPv4 address raw value as an unsigned int
 *
 * @param src
 * @return unsigned int
 */
unsigned int ip4_raw_value(const char *src);

/**
 * @brief Return an IP address value as a string
 *
 * @param af
 * @param src
 * @param dest
 * @return int
 */
int ip_string_value(int af, const char *buf, char *dest);

/**
 * @brief Convert Ethernet address from string to raw bytes
 *
 * @param src
 * @param dest
 * @return int
 */
int eth_raw_value(const char *src, unsigned char *dest);

/**
 * @brief Convert Ethernet address from raw bytes to string
 *
 * @param src
 * @param dest
 * @return int
 */
int eth_string_value(unsigned char *src, char *dest);

/**
 * @brief Return port value from string
 *
 */
unsigned short port_raw_value(const char *src);

#endif
