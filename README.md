# 🐝 tinyfilter

eBPF (XDP) packet filter for the layer 2, 3 and 4.


The filtering mechanism is inspired by [Berkeley packet filters](https://www.ibm.com/docs/en/qsip/7.4?topic=queries-berkeley-packet-filters) but in a simpler way.

## 📖 Build and run

You only need the following requirements (packages):
- `llvm`
- `clang`
- `make`
- `libxdp`

### Debian / Ubuntu

```bash
apt install -y llvm clang make libxdp-dev
```

### RHEL / Fedora

```bash
dnf install -y llvm clang make libxdp-devel
```

You should also have `bpftool` if you need to debug.

To build, run the following command.
```bash
make
```

Then you can run.
```bash
./tinyfilter help
```

## 🤝 Contribute

If you want to help the project, you can follow the guidelines in [CONTRIBUTING.md](./CONTRIBUTING.md).

## 📏 Filters

There are multiple available filters, addresse and port can be source or/and dest.

- Layer 2
  - Ethernet (adresses)
- Layer 3
  - IPv4 (adresses)
  - IPv6 (adresses)
- Layer 4
  - TCP (ports)
  - UDP (ports)
  - ICMP (type)

## ⭐ Use cases

To test the program, I have created two Linux network namespaces (both reachable by each other) following [this steps](https://medium.com/@technbd/creating-network-namespaces-in-linux-system-and-connecting-two-network-namespaces-using-virtual-6031d295f69b).

```bash
# Creates namespaces
ip netns add ns1
ip netns add ns2

# Creates two wirtual network interface peered
ip link add veth1 type veth peer name veth2

# Assigns virtual interface to the namespaces
ip link set veth1 netns ns1
ip link set veth2 netns ns2

# Assigns IP addresses to the virtual interfaces inside the namespaces
ip netns exec ns1 ip addr add 10.10.0.2/24 dev veth1
ip netns exec ns2 ip addr add 10.10.0.3/24 dev veth2

# Enables the virtual interfaces inside the namespaces
ip netns exec ns1 ip link set dev veth1 up
ip netns exec ns2 ip link set dev veth2 up
```

Now everything is setup, you can run the XDP program inside the first network namespace and play with filters.

## 📎 Some examples

#### Load the XDP program

```bash
./tinyfilter load -i veth1
```

If you this error `libxdp: No bpffs found at /sys/fs/bpf`, you can mount the filesystem using the following command.

```bash
mount -t bpf bpf /sys/fs/bpf/
```

#### Load the XDP program with the XDP_PASS return value

```bash
./tinyfilter load -i veth1 -b ./src/tinyfilter_pass.bpf.o
```

#### Filter for TCP from port 10001 to 10002

```bash
./tinyfilter port add -i veth1 --src 10001 --dst 10002 -p tcp
```

#### Filter for SSH port

```bash
./tinyfilter port add -i veth1 --dst 22 -p tcp
```

#### Filter for IPv6 address fe80::9c0c:93ff:fe18:7d1d

```bash
./tinyfilter ip6 add -i veth1 --src fe80::9c0c:93ff:fe18:7d1d
```

## 🎉 Tasks

- [x] Resolve layer 4 protocols strings
- [x] Implement ICMP filters
- [x] Two programs for two default policy (allow / drop)
- [ ] Unpin maps at unload by network interface name
