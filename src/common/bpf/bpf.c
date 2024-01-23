#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>

#include <linux/limits.h>
#include <xdp/libxdp.h>

#include "./bpf.h"

/**
 * @brief Pin base directory for BPF maps/objects
 * 
 */
static const char *pin_root_path = "/sys/fs/bpf";

const char *get_pin_root_path()
{
    return pin_root_path;
}

int common_bpf_unpin_maps_by_prog(struct bpf_object *bpf_object,
    const char *prog_dirname)
{
    char pin_dir[PATH_MAX];
	int err;

    err = snprintf(pin_dir, PATH_MAX, "%s/%s", pin_root_path, prog_dirname);
	if (err < 0)
		return EXIT_FAILURE;
    
    if (access(pin_dir, F_OK) == -1)
        return -1;

    err = bpf_object__unpin_maps(bpf_object, pin_dir);
    if (err)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int common_bpf_pin_maps_by_prog(struct bpf_object *bpf_object,
    const char *prog_dirname)
{
    char pin_dir[PATH_MAX];
	int err;

    err = snprintf(pin_dir, PATH_MAX, "%s/%s", pin_root_path, prog_dirname);
	if (err < 0)
		return EXIT_FAILURE;
    
    // Try to unpin anyway
    common_bpf_unpin_maps_by_prog(bpf_object, prog_dirname);
    
    err = bpf_object__pin_maps(bpf_object, pin_dir);
    if (err) {
        fprintf(stderr, "Error, pinning maps in %s\n", pin_dir);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int common_bpf_pinned_map(const char *prog_dirname,
    const char *map_name)
{
    int err, map_fd;
    char map_path[PATH_MAX];

    err = snprintf(map_path, PATH_MAX, "%s/%s/%s", pin_root_path, prog_dirname,
        map_name);
	if (err < 0)
		return -1;
    
    map_fd = bpf_obj_get(map_path);
    if (map_fd < 0)
        return -1;
    
    return map_fd;
}
