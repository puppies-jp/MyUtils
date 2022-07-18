#include <stdio.h>
#include <stdlib.h>

//#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>

#include "dump.h"

#define PORT 7070

int main(void)
{
    int sockfd /* 自ソケット */,
        new_sockfd;
    struct sockaddr_in host_addr /* 自ホスト情報 */,
        client_addr /* クライアント情報 */;
    socklen_t sin_size;
    int recv_length = 1, yes = 1;
    char buffer[1024];

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        printf("socketの生成に失敗しました"), exit(1);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        printf("socketのSO_REUSEADDR設定に失敗しました。"), exit(1);

    host_addr.sin_family = AF_INET;   /*  */
    host_addr.sin_port = htons(PORT); /* PORT番号を短整数に変換　*/
    /* 0に設定することで現在の自身のIPアドレスが設定される。 */
    host_addr.sin_addr.s_addr = 0;
    memset(&host_addr.sin_zero, 0x00, 8); /* 構造体の残りを0に */

    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
        printf("bindに失敗しました。"), exit(1);

    if (listen(sockfd, 5) == -1)
        printf("ソケット待ち受けに失敗しました。"), exit(1);

    while (true)
    {
        sin_size = sizeof(struct sockaddr_in);
        new_sockfd = accept(
            sockfd,
            (sockaddr *)&client_addr,
            &sin_size);
        if (new_sockfd == -1)
        {
            printf("コネクションの受付に失敗しました。");
            continue;
        }
        printf(
            "コネクションを受け付けました IP: %s, port: %d\n",
            inet_ntoa(client_addr.sin_addr),
            ntohs(client_addr.sin_port));

        send(new_sockfd, "Hello world!\n", 13, 0);

        do
        {
            recv_length = recv(new_sockfd, &buffer, 1024, 0);
            printf("%d byte 受信しました\n", recv_length);
            dump((u_char *)buffer, recv_length);
        } while (recv_length > 0);
        close(new_sockfd);
    }

    return 0;
}