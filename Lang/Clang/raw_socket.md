# 生のソケットを使用する

- 通常のソースではストリームソケットを用いてTCP/IP通信を行うため、
  セッション層の機能までをOSが担っていた。
  生のソケットを使用することで、より下位層を扱うことができる。

```cpp
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
    char buffer[9000];

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
    }
}
```

- 実行結果(telnetで無理やりTCP/IP通信のパケットを送る)

```sh
(base) root@9ea233d8d241:~/Desktop/LeakDetect/sniff# g++ raw_socket_sniff.cpp dump.cpp 
(base) root@9ea233d8d241:~/Desktop/LeakDetect/sniff# 
(base) root@9ea233d8d241:~/Desktop/LeakDetect/sniff# ./a.out 
60 のパケットを受信しました
45 10 00 3C 97 53 40 00 40 06 A5 56 7F 00 00 01 | E..<.S@.@..V....
7F 00 00 01 8F D6 00 17 E0 DD BB BF 00 00 00 00 | ................
A0 02 FF D7 FE 30 00 00 02 04 FF D7 04 02 08 0A | .....0..........
2C 48 9E F2 00 00 00 00 01 03 03 07             | ,H..........
40 のパケットを受信しました
45 10 00 28 00 00 40 00 40 06 3C BE 7F 00 00 01 | E..(..@.@.<.....
7F 00 00 01 00 17 8F D6 00 00 00 00 E0 DD BB C0 | ................
50 14 00 00 85 42 00 00                         | P....B..
60 のパケットを受信しました
45 10 00 3C 79 27 40 00 40 06 C3 82 7F 00 00 01 | E..<y'@.@.......
7F 00 00 01 8F D8 00 17 6D A5 24 EC 00 00 00 00 | ........m.$.....
A0 02 FF D7 FE 30 00 00 02 04 FF D7 04 02 08 0A | .....0..........
2C 48 B4 80 00 00 00 00 01 03 03 07             | ,H..........
(base) root@9ea233d8d241:~/Desktop/LeakDetect/sniff# 

```

- telnet側

```sh
(base) root@9ea233d8d241:/# telnet localhost
Trying 127.0.0.1...
Trying ::1...
telnet: Unable to connect to remote host: Cannot assign requested address
(base) root@9ea233d8d241:/# telnet localhost
Trying 127.0.0.1...
Trying ::1...
telnet: Unable to connect to remote host: Cannot assign requested address
```
