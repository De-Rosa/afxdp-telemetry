# afxdp-telemetry
c++ low-latency telemetry pipeline using AF_XDP, lock-free SPSC queue, eBPF/XDP instrumentation

## attach/detach xdp to eth0
`sudo ip link set dev eth0 xdp obj bin/xdp_pass.o sec xdp`

verify its attached:

`ip -details link show dev eth0`

detach it:

`sudo ip link set dev eth0 xdp off`
