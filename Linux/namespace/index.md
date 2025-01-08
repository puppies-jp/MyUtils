# Linuxのnamespaceについて

コンテナの中核にはlinuxネームスペースが使われる。
この機能によってプロセスの集合毎にリソースを分離することができる。
コンテナ毎に異なるファイルシステムをマウントしたり、コンテナ内から外のプロセスが見えなかったりといった具合。

以下のネームスペース機能がサポートされている。

|ネームスペース|使い方|
|--|--|
|マウント|ファイルシステムの分離。マウント/アンマウントでネームスペース外のシステムに影響しなくなる|
|UTS(Unix Time shareing)|hostnameやdomain nameの分離|
|IPC|System V IPCオブジェクト,POSIX message queueを分離|
|ネットワーク|IP/ルーティング等を分離|
|PID|プロセスIDの集合を分離。PIDネームスペース内で作成されるプロセスから外のプロセスは見えず。また、他とPIDを共有しないので1から振りなおされる。|
|Control Group(cgroup)|cgroupを分離。ネストされたコンテナに使える|
|ユーザ|ユーザID,グループID,ケーパビリティの分離|
|Time|CLOCK_MONOTONIC,CLOCK_BOOTTIMEの分離|

## PID/マウントネームスペースを作成する

こんな感じで`unshare`コマンドでネームスペースを作成できる。
`--pid`でPIDネームスペースを、`--mount`でマウントネームスペースが作成できる。

```bash
# bashがPID 1で実行される。
sudo unshare --pid --mount --fork /bin/bash

# procfsを/procにマウントし直すことができるのは、--mountネームスペースのおかげ
mount -t proc procfs /proc
```
