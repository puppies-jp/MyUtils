#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>

#include "dump.h"
//#include "network_hack.h"

/*
 * g++ pcap_sniff.cpp dump.cpp -lpcap -o pcap_sniff.out
 */

void pcap_fatal(const char *msg, const char *buf = "")
{
    printf("Fatal Error %s: %s\n", msg, buf);
    exit(1);
}

void caught_packet(
    u_char *pargs, const struct pcap_pkthdr *cap_header,
    const u_char *packet);

int main()
{
    struct pcap_pkthdr header;
    const u_char *packet;
    char errbuf[PCAP_ERRBUF_SIZE];
    char *device;
    pcap_t *pcap_handle;
    int i;

    device = pcap_lookupdev(errbuf);
    if (device == NULL)
        pcap_fatal("pcap_lookupdev", errbuf);

    printf("スニッフィング対象 NIC: %s\n", device);
    pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf);
    if (pcap_handle == NULL)
        pcap_fatal("pcap_handle", errbuf);

    /*
        for (int i = 0; i < 3; i++)
        {
            packet = pcap_next(pcap_handle, &header);
            printf("%d byte取得しました\n", header.len);
            dump(packet, header.len);
        }
    */
    pcap_loop(pcap_handle, 3, caught_packet, NULL);
    pcap_close(pcap_handle);
    return 0;
}

void caught_packet(
    u_char *pargs, const struct pcap_pkthdr *cap_header,
    const u_char *packet)
{
    caught_packet_dump(packet, cap_header->len);
}
