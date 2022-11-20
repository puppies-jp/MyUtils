# Linuxについて

Linuxについてのあれこれをメモってく

- [filesystemについて](filesystem)
- [ソケットファイルについて](socketfile)
- [FireWallについて](FireWall/Firewall)

- [便利コマンド](#usefull)
  - [プロセス監視](#watch)
  - [network系](#network)
  - [ファイル転送(rsync)](#rsync)

---

## <a name=usefull>便利コマンドメモ</a>

### <a name=watch>プロセス監視系</a>

- watch

```sh
watch -n <秒> <コマンド>

# 例：1秒おきに docker psを実行する --differenceで差分がハイライトになる
watch -n 1  docker ps -a
watch -n 1 --differences docker ps -a
```

### <a name=network>network系</a>

- telnet

```sh
# ipアドレス、portを指定してserverにコネクトできる。
# tcp接続だけなので、サーバに期待するパケットを送れるわけではない
telnet <ip address> <port>
```

## <a name=rsync>ファイル転送(rsync)</a>

scpについてはとりあえず書かない(概ね知ってるし)

```sh
# 🌟--remove-source-files で送信したファイルの削除を行える(ただし、ディレクトリは消さない)
# 🌟 -z(--compress)らしい
rsync -r -z --remove-source-files <対象ファイル/ディレクトリ> <user>@<host>:<Dest path>

# 🌟 --list-only 転送は行わずにファイルのリストを作成する
rsync -r -z --list-only <対象ファイル/ディレクトリ> <user>@<host>:<Dest path>
```
