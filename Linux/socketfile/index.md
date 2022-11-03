# ソケットファイルについて

UnixドメインソケットはPOSIX系OSで提供されている機能です。  
`コンピュータ内部でしか使えない代わりに、高速に通信が行えます。`
TCP型（ストリーム型）と、UDP型（データグラム型）の両方の使い方ができます。

WindowsではこのUnixドメインソケットをそのまま使うことはできません。 似た概念の機能として、`「名前付きパイプ」`というものが存在します。

- 基本的に`ローカル通信`を宣言して、`ソケットアドレス構造体でファイルパスを指定する`だけ、あとは普通のTCP/IPと同じ
  - [サンプル](https://github.com/puppies-jp/MyUtils/tree/gh-pages/Linux/socketfile)

```cpp
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

// AF_LOCAL/AF_UNIXを指定してローカル通信のソケットを作成
fd_accept = socket(AF_LOCAL, SOCK_STREAM, 0);
struct sockaddr_un sun;

// 🌟ソケットアドレス構造体に接続先(サーバー)を設定
sun.sun_family = AF_LOCAL;// ローカル通信
strcpy(sun.sun_path, UNIX_SOCKET_PATH);// ファイルパス

// 上記設定をソケットに紐づける
ret_code = bind(fd_accept, (const struct sockaddr *)&sockUnix, sizeof(sockUnix));
    
// ソケットに接続待ちを設定する。10はバックログ、同時に何個迄接続要求を受け付けるか。
ret_code = listen(fd_accept, 10);
    
```
