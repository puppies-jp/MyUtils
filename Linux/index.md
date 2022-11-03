# Linuxについて

Linuxについてのあれこれをメモってく

- [filesystemについて](filesystem)
- [ソケットファイルについて](socketfile)

- [便利コマンド](#usefull)
  - [プロセス監視](#watch)
  - [network系](#network)

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