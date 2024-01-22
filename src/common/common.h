#ifndef __COMMON_H__
#define __COMMON_H__

#include <linux/types.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

/**
 * @brief Convert IP from string to its raw value
 * 
 * @param af 
 * @param src 
 * @param dest 
 * @param size 
 * @return int 
 */
int ip_raw_value(int af, const char *src, void *dest, unsigned int size);

/**
 * @brief Return the IPv4 raw value as an unsigned int
 * 
 * @param src 
 * @return unsigned int 
 */
unsigned int ip4_raw_value(const char *src);

#endif
