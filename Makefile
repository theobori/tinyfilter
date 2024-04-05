CC = clang

TARGET = tinyfilter
ARCH = $(shell uname -m | sed 's/x86_64/x86/' | sed 's/aarch64/arm64/')

SRC_DIR = src

USER_C = $(SRC_DIR)/main.c \
		 $(SRC_DIR)/argparse/argparse.c \
		 $(SRC_DIR)/command/load/load.c \
		 $(SRC_DIR)/command/unload/unload.c \
		 $(SRC_DIR)/command/command.c \
		 $(SRC_DIR)/command/common/common.c \
		 $(SRC_DIR)/command/ip/ip.c \
		 $(SRC_DIR)/command/ip6/ip6.c \
		 $(SRC_DIR)/command/eth/eth.c \
		 $(SRC_DIR)/command/port/port.c \
		 $(SRC_DIR)/command/icmp/icmp.c \
		 $(SRC_DIR)/common/common.c \
		 $(SRC_DIR)/common/bpf/bpf.c \
		 $(SRC_DIR)/common/xdp/xdp.c

USER_OBJ = $(USER_C:.c=.o)

BPF_C = $(SRC_DIR)/$(TARGET)_pass.bpf.c \
		$(SRC_DIR)/$(TARGET)_drop.bpf.c
BPF_OBJ = $(BPF_C:.c=.o)

CFLAGS = -Wall -g

all: $(TARGET) $(BPF_OBJ)

$(TARGET): $(USER_OBJ)
	$(CC) -Wall -o $(TARGET) $(USER_OBJ) -lbpf -lelf -lz -lxdp

$(BPF_OBJ): %.o: %.c
	$(CC) \
	    -target bpf \
        -D __TARGET_ARCH_$(ARCH) \
	    -Wall \
	    -O2 -g -o $@ -c $<
	llvm-strip -g $@

clean:
	$(RM) $(USER_OBJ)

fclean: clean
	$(RM) \
		$(BPF_OBJ) \
		$(TARGET)

re: fclean all

.PHONY: \
	all \
	clean \
	fclean \
	re
