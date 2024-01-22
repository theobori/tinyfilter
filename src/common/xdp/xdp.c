#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <linux/limits.h>

#include <xdp/libxdp.h>

#include "../common.h"
#include "../bpf/bpf.h"
#include "./xdp.h"

static const char* section_name = "xdp";

static const xdp_mode_string_t xdp_mode_strings[] = {
    {"native", XDP_MODE_NATIVE},
    {"skb", XDP_MODE_SKB},
    {"hw", XDP_MODE_HW},
};

enum xdp_attach_mode common_xdp_mode_from_name(const char *name)
{
    unsigned int size;
    xdp_mode_string_t xdp_mode_string;

    if (!name)
        return XDP_MODE_SKB;

    size = ARRAY_SIZE(xdp_mode_strings);

    for (unsigned int i = 0; i < size; i++) {
        xdp_mode_string = xdp_mode_strings[i];

        if (!strcmp(xdp_mode_string.name, name))
            return xdp_mode_string.mode;
    }

    return XDP_DEFAULT_MODE;
}

int common_xdp_load(const char *ifname, const char *filename,
    enum xdp_attach_mode mode)
{
	int err;
    unsigned int ifindex;

    struct bpf_object *obj = NULL;
    struct xdp_program *prog = NULL;

	if (!filename || !ifname)
		return EXIT_FAILURE;

    // Get interface index from its name
    ifindex = if_nametoindex(ifname);
	if (!ifindex) {
		fprintf(stderr, "Error, invalid from interface name\n");
		return EXIT_FAILURE;
	}

	// Open the file BPF file object
	prog = xdp_program__open_file(filename, section_name, NULL);
	if (!prog) {
        printf("Error, load xdp prog failed\n");
        return EXIT_FAILURE;
    }

	// Attach the XDP program
	err = xdp_program__attach(prog, ifindex, mode, 0);
	if (err) {
		printf("Error, set xdp fd on %d failed\n", ifindex);
		return err;
	}

    // Get the BPF object
	obj = xdp_program__bpf_obj(prog);
	if (!obj) {
		fprintf(stderr, "Error, failed to open and/or load BPF object from XDP program\n");
		return EXIT_FAILURE;
	}

    // Pin maps
    err = common_bpf_pin_maps_by_prog(obj, ifname);
    if (err) {
		fprintf(stderr, "Error, failed to pin maps\n");
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}

int common_xdp_unload(unsigned int prog_id, const char *ifname,
    enum xdp_attach_mode mode)
{
    int err;
    unsigned int ifindex;

    if (!ifname)
		return EXIT_FAILURE;
    
    // Get interface index from its name
    ifindex = if_nametoindex(ifname);
	if (!ifindex) {
		fprintf(stderr, "Error, invalid from interface name\n");
		return EXIT_FAILURE;
	}

    struct xdp_program *prog = xdp_program__from_id(prog_id);

    // Detach XDP program
    err = xdp_program__detach(prog, ifindex, mode, 0);
    if (err) {
		fprintf(stderr, "Error, failed to detach the XDP program\n");
        return EXIT_FAILURE;
    }

    // TODO: Unpin every maps

    return EXIT_SUCCESS;
}
