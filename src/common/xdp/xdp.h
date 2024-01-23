#ifndef __COMMON_XDP_H__
#define __COMMON_XDP_H__

#include <xdp/libxdp.h>

/**
 * @brief XDP default mode
 * 
 */
#define XDP_DEFAULT_MODE XDP_MODE_SKB

/**
 * @brief XDP mode entry that link
 * its string representation and its value
 * 
 */
typedef struct xdp_mode_string_s {
    char name[10];
    enum xdp_attach_mode mode;
} xdp_mode_string_t;

/**
 * @brief Common XDP program loader
 * 
 * @param ifname 
 * @param filename 
 * @param xdp_mode 
 * @return int 
 */
int common_xdp_load(const char *ifname, const char *filename,
    enum xdp_attach_mode xdp_mode);

/**
 * @brief Common XDP program unloader
 * 
 * @param prog_id 
 * @param ifname 
 * @param mode 
 * @return int 
 */
int common_xdp_unload(unsigned int prog_id, const char *ifname,
    enum xdp_attach_mode mode);

/**
 * @brief Get XDP mode from its string representation
 * 
 * @param name 
 * @return enum xdp_attach_mode 
 */
enum xdp_attach_mode common_xdp_mode_from_name(const char *name);

#endif
