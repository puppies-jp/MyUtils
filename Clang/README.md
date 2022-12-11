# ここでは C/C++ 系の言語についてのあれこれをまとめる

build周り

- [gcc/Cmake について](Cmake)
- [Makefile描き方サンプル](MakeFile)

言語仕様系

- [プリプロセッサ使い方 cheat メモ](preprocessor)
- [コンストラクト/デストラクト](construct)
- [C++の安全なリソース解放について](SafeResource)
- [引数について](FunctionParameter)
- [inline調査](inline#inline)
- [オペレータの定義について](inline#operator)

ossとか環境依存系とか

- [不本意ながらMFCというか、Windows関連](MFC)
- [openGLについて](src/opengl/openGL)
- [boostについて](src/boost/)
- [Google test/mockについて](src/googletest/)
- [printfとかの関数を置き換える](src/hook/)
- [バイナリの使用するライブラリを確認する(ldd)](#ldd)

実装の戦略とか

- [mutex の安全な運用について](mutex)
- [共有メモリについて](src/sharedMem)
- [並行プログラミング戦略](src/pararel/pararel)  
  - [並列ソート処理](src/pthread_sort/sort)
- [lambda式について](lambda)

---

- よく使うソース群
  - [メモリ操作系](#mem)
  - [shared pointer](#sharedPointer)
  - [popenで標準出力/戻り値を取得する](#popen)
  - [エラー出力について](#error)
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

## <a name=ldd>バイナリの使用するライブラリを確認する(ldd)</a>

```bash
(base)root@ubuntu:~# which ls
/usr/bin/ls
(base)root@ubuntu:~# ldd /usr/bin/ls
	linux-vdso.so.1 (0x00007ffd5ff48000)
	libselinux.so.1 => /lib/x86_64-linux-gnu/libselinux.so.1 (0x00007fd86cd1b000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fd86caf3000)
	libpcre2-8.so.0 => /lib/x86_64-linux-gnu/libpcre2-8.so.0 (0x00007fd86ca5c000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fd86cd73000)

```

---
---

## <a name=mem>mem系メモリ操作のあれこれ</a>

C/C++のmem系操作をあれこれ書いてく。

```cpp
#include <string.h>
// 🌟buf2からn byte分をbuf1へコピーする
// 🚨buf1 と buf2 が重なっているときの動作は未定義です。
void *memcpy(void *buf1, const void *buf2, size_t n);

// 🌟buf1 の先頭にbuf2を n byte分 コピー
// 🚨buf1 と buf2 が重なっているときでもbuf1の値からbuf2の値に上書きされる。
void *memmove(void *buf1, const void *buf2, size_t n);
```

---

## <a name=sharedPointer>shared Pointer について</a>

🚨shared_ptrは確かに強力だが、`循環参照`によってリークを発生させる可能性がる。  
(`循環参照`と`対策(weak_ptr)`はそのうち書く)

```cpp
std::shared_ptr<int> ptr;
std::shared_ptr<int> ptr2(new int(0));

//ptr2の所有権をptrにコピー、共有する
ptr=ptr2;

```

```cpp
#include<memory>
#include<iostream>
int main(){
   //空のshared_ptrを作成
   std::shared_ptr<int> ptr;

   {
      //intの所有権を持つ、ptr2を作成
      std::shared_ptr<int> ptr2(new int(0));
      
      //ptr2の所有権をptrにコピー、共有する
      ptr=ptr2;

      *ptr+=10;
      *ptr2+=10;

   }//ここで、ptr2のディストラクタが呼ばれる
    //ptrも同一のメモリに対する所有権を持っているため、まだ解放はされない

   //当然、ptrはまだ使用可能
   std::cout<<"ptr="<<*ptr<<std::endl;  //"ptr=20"と出力

}//ここで、ptrのディストラクタが呼ばれる
 //メモリの所有権を持つポインタがいなくなったので、解放される
```

---
---

## <a name=popen>popenで標準出力/戻り値を取得する</a>

```cpp
#include <stdio.h>

// FILE *popen(const char *command, const char *type);
// int pclose(FILE *stream);


int main()
{
    FILE *fp;
    // 🌟ここでコマンドが実行される
    // "bash -c 'pwd -l'" /* -lが余計で失敗するパターン */
    fp = popen("bash -c 'pwd -l'", "r");
    char buf[256] = {};

    // 🌟標準出力取得(エラーも取得してしまう...)
    // 失敗した段階でエラー取れる方が嬉しいかも。。。
    while (fgets(buf, 256, fp))
        fputs(buf, stdout);

    // 🌟ここで返り値が取得できる。
    int ret = pclose(fp);
    printf("ret: %d\n", ret);
    return 0;
}
```

---

## <a name=error>Errorについて</a>

Linux環境においてC言語系ソースのエラーは引数から0,1で関数の成否が返される。しかし、エラーの原因がどうしてそうなったのか返してくれない。
そこで、`errno.h`を用いることで調べることができる。
慣習的にライブラリ、システムコールの失敗はerrnoに代入される。
(🚨成功時に0となるとは限らないことに注意)
マクロに対応したエラー内容の番号が振られているのでerrnoの番号からエラーの内容を調べることができる。
また、マクロの内容を覚えなくても`void perror(const char *s)`,`char *strerror(int errnum)`を使うことでエラーメッセージの取得ができるようになる。

`🚨飽く迄、関数の返り値でエラーを判定し、errnoからエラーの内容を割り出すように使うこと`

```cpp
// 🌟これをインクルードすることで変数errnoにアクセスできる。
#include <errno.h> 

//これを実行することで、標準エラー出力に出力が可能になる。
#include<stdio.h>
void perror(const char *s);//飽く迄標準エラー出力をしてくれる
// 出力例
// <*s(ユーザー追加の文字列)>:<error message>　

#include <string.h>
char *strerror(int errnum)// errnoのコードを文字列にする
// 使用例
printf("[Error]:%s\n",strerror(errno));
```

## <a name=memory_dump>メモリダンプ</a>

```cpp
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

---

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

---

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

---

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
