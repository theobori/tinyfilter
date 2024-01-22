#ifndef __COMMON_XDP_H__
#define __COMMON_XDP_H__

#include <xdp/libxdp.h>

#define XDP_DEFAULT_MODE XDP_MODE_SKB

typedef struct xdp_mode_string_s {
    char name[10];
    enum xdp_attach_mode mode;
} xdp_mode_string_t;

int common_xdp_load(const char *ifname, const char *filename,
    enum xdp_attach_mode xdp_mode);
int common_xdp_unload(unsigned int prog_id, const char *ifname,
    enum xdp_attach_mode mode);
enum xdp_attach_mode common_xdp_mode_from_name(const char *name);

#endif
