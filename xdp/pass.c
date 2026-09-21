#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <bpf/bpf_helpers.h>

SEC("xdp")
int xdp_pass(struct xdp_md *ctx) {
  // start of packet
  void *data = (void *)(long)ctx->data;
  // end of packet
  void *data_end = (void *)(long)ctx->data_end;

  // first few bytes as ethernet header
  struct ethhdr *eth = data;

  // does packet contain entire ethernet header
  if ((void *)(eth + 1) > data_end) return XDP_ABORTED;
  // check if ipv4
  if (eth->h_proto != __constant_htons(ETH_P_IP)) return XDP_PASS;

  // ip header
  struct iphdr *ip = (void *)(eth + 1);
  // does packet contain entire ip header
  if ((void *)(ip + 1) > data_end) return XDP_ABORTED;

  // udp header
  struct udphdr *udp = (void *)ip + (ip->ihl * 4);
  // does packet contain entire udp header
  if ((void *)(udp + 1) > data_end) return XDP_ABORTED;
  
  return XDP_PASS;
}

char LICENSE[] SEC("license") = "GPL";
