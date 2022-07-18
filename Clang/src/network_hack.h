#ifndef NETWORK_HACK
#define NETWORK_HACK

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ETHER_ADDR_LEN 6
#define ETHER_HDR_LEN 14

struct ether_hdr
{
    unsigned char ether_dest_addr[ETHER_ADDR_LEN]; // 宛先MACアドレス
    unsigned char ether_src_addr[ETHER_ADDR_LEN];  // 送信元MACアドレス
    unsigned short ether_type;                     //イーサネットのパケットタイプ
    /* data */
} ether;

void decode_ethernet(const unsigned char *packet)
{
    const ether_hdr *hdr = (ether_hdr *)packet;
    printf("[第2層::イーサネットヘッダ]\n");
    printf("[送信元]: %02X", hdr->ether_src_addr[0]);
    for (int i = 1; i < ETHER_ADDR_LEN; i++)
        printf(":%02X", hdr->ether_src_addr[i]);

    printf("\t[宛先　]: %02X", hdr->ether_dest_addr[0]);
    for (int i = 1; i < ETHER_ADDR_LEN; i++)
        printf(":%02X", hdr->ether_dest_addr[i]);
    printf("\t [type: %hu]\n", hdr->ether_type);
}

struct ip_hdr
{
    unsigned char ip_version_and_header_len;
    unsigned char ip_tos;          //サービスのタイプ
    unsigned short ip_len;         //トータルの長さ
    unsigned short ip_id;          //識別数値
    unsigned short ip_frag_offset; //フラグメントのオフセットとフラグ
    unsigned char ip_ttl;
    unsigned char ip_type;
    unsigned short ip_check_sum;
    in_addr ip_src_addr;  // 送信元IPアドレス
    in_addr ip_dest_addr; // 宛先IPアドレス
};

void decode_ip(const unsigned char *header_start)
{
    struct ip_hdr *hdr = (struct ip_hdr *)header_start;

    printf("[第3層::IPヘッダ]\n");
    printf("送信元: %s\t", inet_ntoa(hdr->ip_src_addr));
    printf("宛先: %s\n", inet_ntoa(hdr->ip_dest_addr));
    printf("type: %u\t ID: %hu Length: %hu\n",
           (u_int)(hdr->ip_type), ntohs(hdr->ip_id), ntohs(hdr->ip_len));
}

enum
{
    TCP_FIN = 0x01,
    TCP_SYN = 0x02,
    TCP_RST = 0x04,
    TCP_PUSH = 0x08,
    TCP_ACK = 0x10,
    TCP_URG = 0x20,
} TCPFLAG;

struct tcp_hdr
{
    unsigned short src_port;  // 送信元PORT
    unsigned short dest_port; //宛先PORT
    unsigned int seq;
    unsigned int ack;
    unsigned char reserverd : 4;
    unsigned char offset : 4;
    unsigned char flags;
    unsigned short window;
    unsigned short check;
    unsigned short urg_ptr;
};

u_int decode_tcp(const unsigned char *header_start)
{
    tcp_hdr *hdr = (struct tcp_hdr *)header_start;
    u_int header_size = 4 * hdr->offset;
    printf("[第4層::TCPヘッダ]\n");
    printf("[送信元ポート] %hu\t[宛先ポート] %hu\n",
           ntohs(hdr->src_port),
           ntohs(hdr->dest_port));

    printf("[Seq #]: %hu\t Ack #: %u\t header size: %u\t",
           ntohl(hdr->seq), ntohl(hdr->ack), header_size);
    printf("フラグ: ");
    if (hdr->flags & TCP_FIN)
        printf("FIN ");
    if (hdr->flags & TCP_SYN)
        printf("SYN ");
    if (hdr->flags & TCP_RST)
        printf("RST ");
    if (hdr->flags & TCP_PUSH)
        printf("PUSH ");
    if (hdr->flags & TCP_ACK)
        printf("ACK ");
    if (hdr->flags & TCP_URG)
        printf("URG ");

    printf("\n");
    return header_size;
}

#endif