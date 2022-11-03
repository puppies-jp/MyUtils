#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIX_SOCKET_PATH "./test.socket"

// レスポンスの値
enum
{
    OK,
    NG
};

int UpServer();

int main()
{
    return UpServer();
}

int UpServer()
{
    int ret_code = 0;
    char *buf;
    uint32_t buf_len = 0;

    int fd_accept = -1; // 接続受け付け用のFD
    int fd_other = -1;  // sendとかrecv用のFD
    ssize_t size = 0;
    int flags = 0; // MSG_WAITALLとかMSG_NOSIGNALをよく使うけど今回はサンプルなのでフラグは無し

    int response = -1; // レスポンス考えるの面倒だったので0:OK/1:NGで。

    remove(UNIX_SOCKET_PATH); // socket作る前に前回のファイルを消しておく。終了処理でやってもいいけど。

    // ソケットアドレス構造体←今回はここがUNIXドメイン用のやつ
    struct sockaddr_un sockUnix, sockUnix_client;
    memset(&sockUnix, 0, sizeof(sockUnix));
    memset(&sockUnix_client, 0, sizeof(sockUnix_client));
    socklen_t socklen = sizeof(sockUnix);

    // UNIXドメインのソケットを作成
    fd_accept = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd_accept == -1)
    {
        printf("failed to socket(errno:%d, error_str:%s)\n", errno, strerror(errno));
        return -1;
    }

    // ソケットアドレス構造体を設定(ファイル名で紐付ける)
    sockUnix.sun_family = AF_LOCAL;              // UNIXドメイン
    strcpy(sockUnix.sun_path, UNIX_SOCKET_PATH); // UNIXドメインソケットのパスを指定

    // 上記設定をソケットに紐づける
    ret_code = bind(fd_accept, (const struct sockaddr *)&sockUnix, sizeof(sockUnix));
    if (ret_code == -1)
    {
        printf("failed to bind(errno:%d, error_str:%s)\n", errno, strerror(errno));
        close(fd_accept);
        return -1;
    }
    // ソケットに接続待ちを設定する。10はバックログ、同時に何個迄接続要求を受け付けるか。
    ret_code = listen(fd_accept, 10);
    if (ret_code == -1)
    {
        printf("failed to listen(errno:%d, error_str:%s)\n", errno, strerror(errno));
        close(fd_accept);
        return -1;
    }

    // 無限ループのサーバー処理
    while (1)
    {
        printf("accept wating...\n");

        fd_other = accept(fd_accept,
                          (struct sockaddr *)&sockUnix_client,
                          &socklen);
        // ヘッダーの受信(今回はデータサイズのみだが...)
        size = recv(fd_other, &buf_len, sizeof(buf_len), flags);
        if (size < sizeof(buf_len))
        {
            // partial recvパターンも今回はエラーとする
            printf("failed to recv data size(errno:%d, error_str:%s)\n", errno, strerror(errno));
            return NG;
        }
        printf("buf_len = %d\n", buf_len);

        // send/recv用のソケットを閉じてFDを-1に戻しておく
        close(fd_other);
        fd_other = -1;
    }
    return OK;
}