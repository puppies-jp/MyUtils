# Linuxについて

Linuxについてのあれこれをメモってく

- [filesystemについて](filesystem)
- [ソケットファイルについて](socketfile)
- [FireWallについて](FireWall/Firewall)

- [便利コマンド](#usefull)
  - [プロセス監視](#watch)
  - [systemd ファイル作成](#systemd)
  - [network系](#network)
  - [ファイル転送(rsync)](#rsync)
  - [ssh接続について](#ssh)

- [deb/rpmファイルをインストールする](#pkgInstall)

---

## <a name=usefull>便利コマンドメモ</a>

### <a name=ssh>sshについて</a>

```sh
# パーミッションを変更する(読み書きできるとアクセスできないとかある)
chmod 400 <key>

# 公開鍵を指定してアクセス
ssh -i ~/.ssh/id_rsa user@hostname
ssh -i "<キー>.pem" user@host
```

- Port Forwardingについて

```sh
# 🌟トンネル接続(通常の接続では自身のホストからの接続のみフォワディングされる。)
# ほかデバイスから経由する場合は "-g(gateway ports)"オプションをつける)
# 自ホスト(host0:50022) -> 踏み台(host1) -> 転送先(host2:22)
ssh <踏み台ユーザ>@<踏み台> -L 50022:<転送先>:22
# 👆 sshをautossh に置き換えることで、接続が切断されても自動で再接続される。

# scp 自ホスト -> 転送先へ
scp -P 50022 <ファイル>  <転送先ユーザ>@<自ホスト>:<パス> 

## 🌟リモートから自ホストを経由して接続先向けにポートを転送する
# 接続元(host1:22) <- 自ホスト(host0) <- アクセス先(host2:50022)
ssh user@<host2> -R 50022:host1:22

```

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
とりあえず、`rsync`は`scp`と違い送信元,送信先のどちらかをローカルで指定する
必要がある。  
どこかのサーバを経由する場合、sshでポート転送をする必要がある。

`scp`と`rsync`とで差分を取ったりする分、CPU性能が要求される。  
しかし、転送速度は早いらしい。(効率的にCPUを使ってるんだろう。)

```sh
# 🌟--remove-source-files で送信したファイルの削除を行える(ただし、ディレクトリは消さない)
# 🌟 -z(--compress)らしい
# 🌟 -t オプションを指定しない場合、通常転送した後で転送元、転送先で更新日が実行時にされてしまう。そこで-tオプションをつけることで更新日時を保持したままにすることができる
rsync -r -z -t --remove-source-files <対象ファイル/ディレクトリ> <user>@<host>:<Dest path>

# 🌟 --list-only 転送は行わずにファイルのリストを作成する
rsync -r -z --list-only <対象ファイル/ディレクトリ> <user>@<host>:<Dest path>
```

---

## <a name=systemd>Serviceファイルの書き方</a>

[参照](https://qiita.com/masami256/items/ef0f23125cf8255e4857)

基本的にUnit/Service/Installの3つのセクションに分類される

### Unitセクションについて

サービス起動の前後を設定したり、説明を書いたりするセクション

> man 5 systemd.unit

```conf
[Unit]
# 説明を書いたり
Description=Load dump capture kernel

# Before/Afterを書くことでサービス起動の前後を指定できる
Before=sysinit.target shutdown.target
After=local-fs.target

# 起動時に必要なサービスとかを書くらしい、、
Wants=bar.service
Requires=foo.service
```

### Servicesセクションについて

プロセスのタイプ、実行/再起動とかの設定を書き込める
詳細は以下コマンドで確認できる

> man 5 systemd.service

```conf
# sshdだとこんな感じ
[Service]
# -Dオプションをつけて起動
ExecStart=/usr/bin/sshd -D
# 🚨sighupでプロセスを殺しているだけ
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process
Restart=always
```

### Installセクション

```conf
[Install]
# 🌟UnitセクションのWant/Requiredと同じらしい
WantedBy=sysinit.target
RequiredBy=sysinit.target
```

## <a name=pkgInstall>deb/rpmファイルをインストールする</a>

- dpkg(Debian系(apt))

```bash
sudo apt install --download-only パッケージ名
sudo apt install ./ファイル名.deb
sudo apt remove パッケージ名

# install
dpkg -i ファイル名.deb
# 削除
dpkg -r パッケージ名
```

- rpm(RedHat系(yum))

```bash
# yumでパッケージをダウンロードする
yum install パッケージ --downloadonly --downloaddir=ダウンロードしたrpmの格納先パス

# ファイルを指定した場合、yumは依存するパッケージはオンラインで取ろうとしてしまうのに注意
yum localinstall ファイル名.rpm
yum localinstall ダウンロードしたrpmの格納先パス/*

# -i :インストール,-v :詳細表示,-h :進捗表示  
# -test : 実際にはインストールやアップデートを行わず、コマンドのテストを行う
rpm -ivh [–test] パッケージファイル名
# -U: 更新する
rpm -Uvh [–test] ファイル名
# パッケージをアンインストールする
rpm -evh パッケージ名
```
