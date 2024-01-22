#ifndef __COMMON_BPF_H__
#define __COMMON_BPF_H__

#include <bpf/bpf.h>
#include <bpf/libbpf.h>

int common_bpf_unpin_maps_by_prog(struct bpf_object *bpf_object,
    const char *prog_dirname);
int common_bpf_pin_maps_by_prog(struct bpf_object *bpf_object,
    const char *prog_dirname);
const char *get_pin_root_path();
int common_bpf_pinned_map(const char *prog_dirname,
    const char *map_name);

#endif
