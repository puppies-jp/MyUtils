#include <stdio.h>
#include "dump.h"
#include "network_hack.h"

/*
引数のアドレスから16byte単位で改行し、16進数とasciiコード単位で出力する関数
*/
void dump(const unsigned char *data_buffer, const unsigned int length)
{
    unsigned char byte;
    unsigned int i, j;

    for (i = 0; i < length; i++)
    {
        byte = data_buffer[i];
        printf("%02X ", byte);

        // 16byte表示した or 16byte以下で末尾に到達した。
        if (((i % 16) == 15) || (i == length - 1))
        {
            // 不足分の埋め草を出力する。
            for (j = 0; j < 15 - (i % 16); j++)
                printf("   ");

            // asciiコードの文字列を出力する
            printf("| ");

            // 何文字表示したかを16byte単位で丸めてj -> i の値になるまでループ
            for (j = i - (i % 16); j <= i; j++)
            {
                byte = data_buffer[j];
                if ((byte > 31) && (byte < 127))
                    printf("%c", byte); /* 印字可 */
                else
                    printf("."); /*　印字不可　*/
            }
            printf("\n");
        }
    }
}

void caught_packet_dump(const u_char *packet, const uint length)
{
    printf("======= %d byteのパケットを受信しました。\n", length);
    decode_ethernet(packet);
    decode_ip(packet + sizeof(ether_hdr));
    u_int tcp_len = decode_tcp(packet + sizeof(ether_hdr) + sizeof(ip_hdr));

    u_int total_header_size = sizeof(ether_hdr) + sizeof(ip_hdr) + tcp_len;
    unsigned char *pkt_data = (unsigned char *)packet + total_header_size;
    u_int packet_len = length - total_header_size;
    if (packet_len > 0)
    {
        printf("%u バイトのパケットデータ\n", packet_len);
        dump(pkt_data, packet_len);
    }
    else
    {
        printf("パケットデータがありません");
    }
}

/* test用関数
int main()
{
    char *buffer = "this is test.\n Is this dumper right?";
    dump(buffer, 37);
    return 0;
}
*/