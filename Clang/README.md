# ここでは C/C++ 系の言語についてのあれこれをまとめる

- [Cmake について](Cmake)
- [コンストラクト/デストラクト](construct)
- [mutex の安全な運用について](mutex)
- [プリプロセッサ使い方 cheat メモ](preprocessor)
- [C++の安全なリソース解放について](SafeResource)
- [引数について](FunctionParameter)
- [inline調査](inline)
- [不本意ながらMFCというか、Windows関連](MFC)

---

- よく使うソース群
  - [メモリダンプ](#memory_dump)
  - [ディレクトリ有無チェック](#is_dir)
  - [ネットワーク系](#network)
    - [ソケットについて](#socket)
    - [サーバサイド](#server)
    - [クライアントサイド](#client)
  - [パスワード暗号化](#crypt)

- 小技系
  - [ダイレクトパラメータアクセス](#direct)
  - [ソースコードに直でブレークポイントを貼る](#breakPoint)

---

## <a name=memory_dump>メモリダンプ</a>

```c
#include "stdio.h"

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
            for (j = 0; j < 15 - (i % 16); j++) printf("   ");

            // asciiコードの文字列を出力する
            printf("| ");

            // 何文字表示したかを16byte単位で丸めてj -> i の値になるまでループ
            for (j = i - (i % 16); j <= i; j++)
            {
                byte = data_buffer[j];
                if ((byte > 31) && (byte < 127))printf("%c", byte);/* 印字可 */
                else printf(".");/*　印字不可　*/
            }
            printf("\n");
        }
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
```

---
## <a name=is_dir>ディレクトリチェック</a>

```c++
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
    // std=c++17から使える
    // 引数にディレクトリのパスを渡すと存在の有無をチェックしてくれる。
    auto isDirectry1 = fs::is_directory(std::string("./"));
        std::cout << "./ :" << (isDirectry1 == true) << std::endl;
}
```

```sh
(base) root@9ea233d8d241:~/Desktop/MyUtils/Clang/src# g++ isDir.cpp -o dir.out -std=c++17
(base) root@9ea233d8d241:~/Desktop/MyUtils/Clang/src# ./dir.out 
./ :1
```

---
## <a name=breakPoint>ブレークポイント</a>

```cpp
#include<signal.h>
raise(SIGTRAP)
```

## <a name=network>ネットワーク系</a>

### <a name=socket>ソケットについて</a>

- ソケットは`OSI参照モデル`における、`セッション層`の技術でソケットの種類によってトランスポート層の構造が決定される。(TCPやUDPなどに)  
使い方としては以下のようにFile記述子のようにして作成する。
`/usr/include/x86_64-linux-gnu/bits/socket.h`

- [生ソケットの使用について](raw_socket)

```cpp
/* /usr/include/x86_64-linux-gnu/bits/socket.h参照
* int domain
*            ソケットのプロトコルファミリのこと IPプロトコルや
*            アマチュア無線のプロトコルなどを指定する。
*            (Protocol families　(参照: /usr/include/x86_64-linux-gnu/bits/socket.h参照))
* int type 
*          ソケットの型のこと(Type of sockets参照)
*          SOCK_STREAM(1), ストリームソケット(双方向)
*          SOCK_DGRAM(2), データグラムソケット(一方通行で、送った順序で届くとは限らない)
*          他 bits/socket_type.hに定義されている。
* int protocol 
*          プロトコルファミリ内に複数のプロトコルが存在する場合指定する。
*          ほとんどの場合、1つしかないため0となる。
*/
int sockfd = socket(int domain, int type, int protocol)
```

### <a name=server>サーバーサイド</a>

- [参考プログラム](https://github.com/puppies-jp/MyUtils/blob/gh-pages/Clang/src/simple_server.cpp)

```cpp
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
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
    struct sockaddr_in host_addr; /* 自ホスト情報 */
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

    // 自ホストとソケットをバインドする
    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
        printf("bindに失敗しました。"), exit(1);

    // NOTE:listenでソケットへの待受をする
    //       int listen(int sockfd, int backlog);
    //      sockfd が参照するソケットを接続待ちソケット (passive socket) として印をつける。 
    //      接続待ちソケットとは、 accept を使って到着した接続要求を受け付けるのに使用されるソケットである。 
    //      backlog 引数は、 sockfd についての保留中の接続のキューの最大長を指定する。 キューがいっぱいの状態で接続要求が到着すると、クライアントは "ECONNREFUSED" というエラーを受け取る。
    if (listen(sockfd, 5) == -1)
        printf("ソケット待ち受けに失敗しました。"), exit(1);

    while (true)
    {
        struct sockaddr_in client_addr; /* クライアント情報 */
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
            dump(buffer, recv_length);
        } while (recv_length > 0);
        close(new_sockfd);
    }

    return 0;
}
```

- telnetで接続した場合

```sh
(base) root@9ea233d8d241:/# telnet localhost 7070
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Hello world!
1
```

- コンソール出力

```sh
(base) root@9ea233d8d241:~/Desktop/LeakDetect/sniff# g++ dump.cpp  simple_server.cpp -o a.out
(base) root@9ea233d8d241:~/Desktop/LeakDetect/sniff# 
(base) root@9ea233d8d241:~/Desktop/LeakDetect/sniff# ./a.out 
コネクションを受け付けました IP: 127.0.0.1, port: 59228
3 byte 受信しました
31 0D 0A                                        | 1..
3 byte 受信しました
32 0D 0A                                        | 2..
3 byte 受信しました
```

### <a name=client>クライアントサイド</a>

- こんな感じで実装することでclient側が実装できる

```cpp
    int port = 7070;
    char *mes = "hello server";
    char *ip = "127.0.0.1";
    int len = strlen(mes);
    int sock;

    // 🌟ここでソケットを作成
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        error_message(__LINE__);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    // 🌟ここでソケットを接続
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        error_message(__LINE__);

    while (total < len)
    {
        // 🌟受信
        if ((num = recv(sock, buf, 49, 0)) <= 0)
            error_message(__LINE__);

        total += num;
        buf[num] = '\0';
        printf("%s", buf);
    }
    // 🌟送信
    if (send(sock, mes, len, 0) != len)
        error_message(__LINE__);

```

### <a name=direct>ダイレクトパラメータアクセス</a>

- printfに`$`を使ってやることで、引数の何番目の数字を使用するのかを直で指定できる

- 使い方はこんな感じ

```c
printf("%2$d",1,2,3);
```

- 出力例
  - ⚠️他の`%d`などのアクセス順に影響してない

```sh
(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# cat dyrectPrm.c 
#include<stdio.h>

int main()
{
    printf(
        "%d %d %1$d %d %d %d \n
        arg2: %2$d\n"
        ,1,2,3,4,5);
        return 0;
}

(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# ./a.out
1 2 1 3 4 5 
arg2: 2
(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# 
```

## <a name="crypt">password暗号化</a>

- [参照](https://github.com/puppies-jp/LeakDetect/blob/master/crypt/crypt_test.cpp)

```c
#include <unistd.h>
#include <stdio.h>

/* 🌟コンパイルオプションにcryptをリンクすること
 * g++ crypt_test.cpp -o crypt.out -lcrypt
 */

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage %s <password> <salt>\n", argv[0]);
        return 1;
    }
    printf("password = '%s' , salt = '%s'\n", argv[1], argv[2]);
    printf("hash => '%s'\n", crypt(argv[1], argv[2]));
}
```