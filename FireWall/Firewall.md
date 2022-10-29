# networkとfirewallについて

知ってることは書かない、知識がふわっとしてるものだけ書く

- [networkについて](#network)
- [firewall,セキュリティについて](#sec)

Dockerのネットワークについては以下を参照

- [Dockerのネットワークについて](../Docker/dockerNetwork)

## <a name=network>networkについてあれこれ</a>

- ブリッジ接続/ブリッジモード(ルータ)とは

複数のネットワークを繋げて一つのネットワークのように扱うこと。
ルータのブリッジモードとはちょっと毛色が違う別の話

ブリッジ接続: 複数のネットワークを繋げて一つのネットワークぽく扱うこと

```text
+ ------[ Network ] --------------------------------+
|                                                   |
|  +--[ Network1 ]----+        +---[ Network2 ]--+  |
|  | +-----+          |bridge  |      +-----+    |  |
|  | |PC1  |*--------*|*------*|-----*| PC3 |    |  |
|  | +-----+     |    |        |      +-----+    |  |
|  |             |    |        |                 |  |
|  | +-----+     |    |        +-----------------+  |
|  | |PC2  |*----|    |                             |
|  | +-----+          |                             |
|  |                  |                             |
|  +------------------+                             |
|                                                   |
+---------------------------------------------------+

```


🚨 以下のようにつなぐと `PC1,2`は`router1`のネットワークを形成し、
   `PC3`は`router2`でネットワークを形成される。

この場合、 `PC1,2,3`はwebにアクセスできるが、  
`PC1,2`と`PC3`間では違うネットワークにいるため、アクセスができなくなる。

`rounter1`を`スイッチングハブ`と置き換えることで、  
`PC3`と`PC1,2`でアクセスできるようになる。  
  
しかし、`router1`を`bridgeモード`で動かすことで、  
`router`としての機能をオフにして`スイッチングハブ`として使うことができる。

```text

🚨 この状態だと network1、2間でのアクセスができない
+---[ Network 1 ]--------+
|                        |
|+-----+      +-------+  | 
||PC 1 |*----*|router1|  |
|+-----+      +-------+  |
|               *   *    |
|+-----+        |   |    |             
||PC 2 |*------ |   |    |             
|+-----+            |    |
+-------------------|----+
                    |
+--[ Network 2 ]----|---------+
|                   *         |
| +-----+         +-------+   |      +~~~~~~+
| |PC3  |*-------*|router2|*-*|*---- |  web |
| +-----+         +-------+   |      +~~~~~~+
+-----------------------------+

```

---

## <a name=sec>firewall-cmd, iptables などのコマンドについてまとめる。</a>

- 用途
  1. 接続の可否などを分けたいシステムの network 設定を行う。
  2. 開発したシステムのテストで接続の可否を切り替えてテストを行うときに設定をすることでソースコードを変更することなく切り替えることができる。

[iptables](#iptables)  
[filewall-cmd](#firewall)  
[home](https://puppies-jp.github.io/MyUtils/)

[OSI参照モデル 関連リンク](../BinaryHack/Hacking/02_Network)

---

## <a name=iptables>iptables</a>

- コマンド例

```shell
iptables 〈テーブル〉〈コマンド〉 〈マッチ〉〈ターゲット〉

# INPUT に対してプロトコルが tcp のものを許可する設定
# optionを追加することで特定のIPのみを弾いたりすることができる。
iptables -t filter -A INPUT -p tcp -j ACCEPT
```

- Chain とは

  - INPUT、OUTPUT、FORWARD という３つの経路に対して、それぞれ通過させるパケットのルールを設定する。この３つの経路を「チェイン」と呼ぶ。

- target
  - target に関しては許可するのか拒否するのかログをとるのか？など。条件にマッチするパケットの処理の行き先をどうしますか？という意味だ。
- prot
  - prot はプロトコルの種類
- opt
  - opt はオプションが記述
- source
  - source はどこから来たのか
- destination

  - destination はどこへ向うか？

### ルールの適用順序

- iptables は上から順番にルールを適応する。例えば、下記のこの項目を真っ先に設定すると全てのパケットログがとられてしまう
  > LOG all -- anywhere anywhere LOG level warning prefix "drop_packet: "

### 設定の確認方法

```shell
iptables [サブコマンド] [パラメータ]
```

| サブコマンド       | 説明                                                     |
| :----------------- | :------------------------------------------------------- |
| -L, --list [CHAIN] | チェイン CHAIN（省略した場合はすべて）のルールを一覧表示 |

| パラメータ    | 説明                                             |
| :------------ | :----------------------------------------------- |
| -v, --verbose | 詳細に出力する                                   |
| -n, --numeric | 名前解決せず IP アドレスやポートを数値で出力する |
| -x, --exact   | 正確な数値を表示する                             |

### 用語などの一覧

| table  | 説明                                                                                                                                                                                                                                 |
| :----- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| filter | 一般的なフィルタテーブル パケットの通過や遮断といった制御をします。通常ほとんどの設定はこのテーブルに記述します。                                                                                                                    |
| nat    | マスカレードなどを記述するテーブル 送信先や送信元といったパケットの中身を書き換える際に利用します。各通信をローカルネットワーク上のサーバへ振り分けるルーターとして機能させることができます。                                        |
| mangle | このテーブルを使うと Quality of Service などが設定可能 TOS はパケット処理の優先度付けを行い、通信品質を制御する際に利用されます。                                                                                                    |
| Raw    | 用途は mangle テーブルのように特定のパケットにマークを付けることですが、Raw テーブルでは追跡を除外するようマークを付けます。つまり、特定の通信をファイアウォールで処理せずに他の機材へ通したりといった経路制御する場合に利用します。 |

| chain       | 説明                       |
| :---------- | :------------------------- |
| INPUT       | 入ってくるパケットに関して |
| OUTPUT      | 出てゆくパケットに関して   |
| FORWARD     | パケットの転送             |
| PREROUTING  | 受信時にアドレスを変換     |
| POSTROUTING | 送信時にアドレスを変換     |

| target   | 説明                                    |
| :------- | :-------------------------------------- |
| ACCEPT   | パケットを許可                          |
| DROP     | パケットを破棄。応答を返さない。        |
| REJECT   | パケットを拒否し、ICMP メッセージを返信 |
| REDIRECT | 特定ポートにリダイレクト                |

| parameter            | 説明                                                                                                        |
| :------------------- | :---------------------------------------------------------------------------------------------------------- |
| -s (--source)        | パケットの送信元を指定。特定の IP（192.168.0.1）や範囲（192.168.0.0/24）を指定する。                        |
| -d (--destination)   | パケットの宛先を指定。指定方法は-s と同じ。                                                                 |
| -p (--protocol)      | チェックされるパケットのプロトコル。 指定できるプロトコルは、 tcp、udp、icmp、all のいずれか 1 つか、数値。 |
| -i (--in-interface)  | パケットを受信することになるインターフェース名。eth0、eth1 など。                                           |
| -o (--out-interface) | 送信先インターフェース名を指定。                                                                            |
| -j (--jump)          | ターゲット（ACCEPT、DROP、REJECT）を指定                                                                    |

### 使用例

```shell
# ポート80で受信したTCPの受信パケットを破棄
iptables -A INPUT -p tcp --dport 80 -j DROP

# < `-s(送信元)`|`-d(宛先)` >
# < `--sport(送信元ポート)`|`--dport(宛先ポート)`>への
iptables -A INPUT -p tcp -s <IPaddr> --dport 80 -j DROP

# IPマスカレードの指定
iptables -t nat -A POSTROUTING -s 192.168.1.0/24 -j MASQUERADE
```

```shell
#!/bin/bash

# 設定をクリア
iptables -F
iptables -X
# ポリシー設定
iptables -P INPUT   DROP # defaultでinputをdropする
iptables -P FORWARD DROP　# defaultでforwardをdropする
iptables -P OUTPUT  ACCEPT # defaultでoutputは通す

# ほかACCEPTやDROPなどユーザによる設定

#ローカルループバックの接続を許可する。(loはloopbackの略)
iptables -A INPUT -i lo -j ACCEPT
#こちらから求めたパケットは許可する。
# (ESTABLISHED,RELATEDな接続は許可する。)
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
#それ以外はログを残す。
iptables -A INPUT -j LOG --log-prefix "drop_packet:"
```

### 設定の保存方法(※環境により異なる)

```shell
# iptables-saveコマンドを使い、標準出力されるものをリダイレクトで保存
iptables-save > /etc/myiptables-set
# 下記入力で復元
iptables-restore < /etc/myiptables-set
```

- 参考
  - https://qiita.com/Tocyuki/items/6d90a1ec4dd8e991a1ce
  - https://eng-entrance.com/linux-firewall

---

## <a name=firewall>firewall-cmd</a>

- ルールについて

  - ランタイムルール(揮発性)
    - メモリ上に保存されるルール。
      firewalld をリスタートすると消える。
  - パーマネントルール
    - ファイル(/etc/firewalld/zones 配下)に保存されるルール。
      firewalld をリスタートすると、ファイルからルールが読み込まれメモリに展開される。

- この markdown について
  - firewall-cmd は下記のような位置付けである。
    では、なぜ firewall-cmd を使うのか、zone 設定ができるから(おそらく iptables でもできる)
  - ここでは、Firewall の zone について記述をしていこうと思う

```shell
       CentOS7         Ubuntu16.04        openSUSE(Leap 42.3)
  +--------------+    +------------+    +---------------------+   -*-
  | firewall-cmd |    |     ufw    |    |    SUSEfirewall2    |    |
  +--------------+    +------------+    +---------------------+    |
                                                                 ユーザ空間
  +-----------------------------------------------------------+    |
  |                     iptables command                      |    |
  +-----------------------------------------------------------+   -*-

  +-----------------------------------------------------------+   -*-
  |                                                           |    |
  |                    OS(Netfilter)                          |  カーネル空間
  |                                                           |    |
  +-----------------------------------------------------------+   -*-
```

### ゾーンについて

- デフォルトゾーンの確認方法(--get-default-zone)

> firewall-cmd --get-default-zone #(default は public である。)

- デフォルトゾーンの変更方法(--set-default-zone)

```shell
デフォルトのゾーンをtrustedに変更する。
[root@server ~]# firewall-cmd --set-default-zone=trusted
success

デフォルトのゾーンを確認する。trustedに変更されたことがわかる。
[root@server ~]# firewall-cmd --get-default-zone
trusted

12345番ポートをアクセス可能に設定する。
[root@server ~]# firewall-cmd --add-port=12345/tcp
success

trustedゾーンの状態を確認する。12345番ポートがアクセス可能になったことがわかる(★印)。
[root@server ~]# firewall-cmd --zone=trusted --list-all
trusted (active)
  target: ACCEPT
  icmp-block-inversion: no
  interfaces: eth0
  sources:
  services:
  ports: ★12345/tcp
  protocols:
  masquerade: no
  forward-ports:
  sourceports:
  icmp-blocks:
  rich rules:
```

- ゾーンの状態を確認する方法(--list-all-zones)

```shell
# 全てのゾーンの状態確認
firewall-cmd --list-all-zones
# 特定ゾーンの状態確認
firewall-cmd --zone=public --list-all
```

- ゾーンに属するインタフェースの表示、変更方法

```shell
publicゾーンにeth0,eth1の2つのインタフェースが存在することがわかる。
[root@server ~]# firewall-cmd --get-active-zones
public
  interfaces: eth0 eth1

eth1をhomeゾーンに変更する。(※パーマメントではない)
[root@server ~]# firewall-cmd --zone=home --change-interface=eth1
The interface is under control of NetworkManager, setting zone to 'home'.
success

eth0がpublicゾーン、eth1がhomeゾーンに所属することがわかる。
[root@server ~]# firewall-cmd --get-active-zones
home
  interfaces: eth1
public
  interfaces: eth0
```

- Nic のゾーンを永続的に変更するには...

```shell
# /etc/sysconfig/network-scripts/ifcfg-<NIC名> に対して
# ZONE=<ゾーン名> を追加してnetworkのreload
systemctl restart network
```

- 参考

  - https://qiita.com/hana_shin/items/bd9ba363ba06882e1fab
  - https://qiita.com/miyase256/items/1dd937f1c6d9341e5635

---

[To top](#head)
