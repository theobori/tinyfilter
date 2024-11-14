#ifndef __COMMON_BPF_H__
#define __COMMON_BPF_H__

#include <bpf/bpf.h>
#include <bpf/libbpf.h>

/**
 * @brief Unpin a BPF map by program name
 *
 * @param bpf_object
 * @param prog_dirname
 * @return int
 */
int common_bpf_unpin_maps_by_prog(struct bpf_object *bpf_object,
                                  const char *prog_dirname);

/**
 * @brief Pin a BPF map by program name
 *
 * @param bpf_object
 * @param prog_dirname
 * @return int
 */
int common_bpf_pin_maps_by_prog(struct bpf_object *bpf_object,
                                const char *prog_dirname);

/**
 * @brief Get the pin root path object
 *
 * @return const char*
 */
const char *get_pin_root_path();

/**
 * @brief Get a BPF map file descriptor
 *
 * @param prog_dirname
 * @param map_name
 * @return int
 */
int common_bpf_pinned_map(const char *prog_dirname, const char *map_name);

#endif
