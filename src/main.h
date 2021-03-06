/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2015 Vladimir Medvedkin <medvedkinv@gmail.com>
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON AND
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PRF_MAIN_H_
#define _PRF_MAIN_H_

#ifdef RTE_EXEC_ENV_BAREMETAL
#define MAIN _main
#else
#define MAIN main
#endif

int MAIN(int argc, char **argv);

#define PRF_ARRAY_SIZE(x)		(sizeof(x)/sizeof(x[0]))
#define PRF_TCP_HASH_NAMESIZE	32
#define PRF_MAX_PKT_BURST		32
#define PRF_MAX_PORTS		2
#define PRF_MEMPOOL_CACHE_SIZE	256
#define PRF_SEC_CTX_MAX_RULES	8
#define PRF_SOCKET0			0

struct prf_lcore_stats {
	uint64_t rx_pkts;
	uint64_t tx_pkts;
	uint64_t malformed;
	uint64_t bad_csum;
	uint64_t frags;
	uint64_t bad_flags;
	uint64_t rst_set;
	uint64_t state_match;
	uint64_t state_mismatch;
	uint64_t cookies_sent;
	uint64_t cookies_rcv;
	uint32_t src_track_overflow;
	uint32_t inserts;
	uint32_t removals;
	uint32_t tw_reuse;
	uint32_t bad_seq_ack;
	uint32_t chained_lookup;
	uint32_t chained_states;
	uint32_t no_mem_pool;
	uint32_t states_counter;
	uint32_t embrionic_counter;
	uint64_t acl_stat[PRF_ACL_MAX_RULES];
} __rte_cache_aligned;

extern int8_t prf_dst_ports[PRF_MAX_PORTS];

struct prf_mbuf_table {
	struct rte_mbuf *m_table[PRF_MAX_PKT_BURST];
};

struct prf_lcore_conf {
	struct prf_ipv4_tcp_hash    *tcp_hash;
	uint64_t		timer;
	uint32_t		bucket_pair_nb;
	uint8_t			queue_id;
	uint8_t			core_role;
	unsigned		len[PRF_MAX_PORTS];
	struct prf_mbuf_table	tx_mbufs[PRF_MAX_PORTS] __rte_cache_aligned;
	struct prf_sec_ctx_rule	rules[PRF_SEC_CTX_MAX_RULES] __rte_cache_aligned;
	struct prf_lcore_stats	stats __rte_cache_aligned;
} __rte_cache_aligned;

extern uint64_t prf_tsc_hz;
extern struct prf_lcore_conf prf_lcore_conf[RTE_MAX_LCORE] __rte_cache_aligned;
extern int prf_mastercore_id;
extern int prf_primarycore_id;
extern int prf_nb_fwd_cores;
extern int prf_nb_worker_cores;

extern struct rte_mempool *prf_pktmbuf_pool;
extern struct rte_mempool *prf_tcp_ent_pool;
extern struct rte_mempool *prf_src_track_pool;

void prf_send_packet(struct rte_mbuf *m, struct prf_lcore_conf *conf, uint8_t port);

#endif /* _PRF_MAIN_H_ */
