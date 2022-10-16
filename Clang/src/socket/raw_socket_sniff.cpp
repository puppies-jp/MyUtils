#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>

#include "dump.h"

int main(void)
{
    int sockfd /* 自ソケット */,
        recv_length;
    u_char buffer[9000];

    // NOTE: 生のソケットを生成するにはroot権限が必要
    // IPPROTO_TCP,IPPROTO_UDP,IPPROTO_ICMPなどが使える
    if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
        printf("ソケットの生成に失敗しました");
        exit(1);
    }

    for (int i = 0; i < 3; i++)
    {
        recv_length = recv(sockfd, buffer, 8000, 0);
        printf("%d のパケットを受信しました\n", recv_length);
        dump(buffer, recv_length);
        // caught_packet_dump((u_char *)buffer, recv_length);
    }
}