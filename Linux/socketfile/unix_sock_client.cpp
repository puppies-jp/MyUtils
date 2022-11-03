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
#define SEND_SIZE_MAX 256

// レスポンスの値
enum
{
    OK,
    NG
};

int unix_client();

int main()
{
    return unix_client();
}

int unix_client()
{

    int ret_code = 0;
    char buf[SEND_SIZE_MAX];
    uint32_t buf_len = 0;

    int fd = -1;
    ssize_t size = 0;
    int flags = 0; // MSG_WAITALLとかMSG_NOSIGNALをよく使うけど今回はサンプルなのでフラグは無し

    int response = -1; // レスポンス考えるの面倒だったので0:OK/1:NGで。

    // ソケットアドレス構造体
    struct sockaddr_un sun;
    memset(&sun, 0, sizeof(sun));

    // UNIXドメインのソケットを作成
    fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("failed to create_socket(errno=%d:%s)\n", errno, strerror(errno));
        return -1;
    }

    // ソケットアドレス構造体に接続先(サーバー)を設定
    sun.sun_family = AF_LOCAL;
    strcpy(sun.sun_path, UNIX_SOCKET_PATH);

    // 上記設定を用いてサーバーに接続
    ret_code = connect(fd, (const struct sockaddr *)&sun, sizeof(sun));
    if (ret_code == -1)
    {
        printf("failed to create_socket(errno:%d, error_str:%s)\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

    // データサイズの送信(ヘッダーの構造とか考えなきゃだめだけど今回は省略。データサイズのみ送る)
    buf_len = 0x31;
    size = send(fd, &buf_len, sizeof(buf_len), flags);
    if (size < sizeof(buf_len))
    {
        // partial sendパターンも今回はエラーとする
        printf("failed to send data size(errno:%d, error_str:%s)\n", errno, strerror(errno));
        close(fd);
        return -1;
    }
    // ソケットを閉じる
    close(fd);

    return OK;
}