# Linux/network namespace

`network namespace`を使って仮想ネットワークを作成する

## network namespace を作成/削除する

```bash
#sudo ip netns add namespace名
sudo ip netns add helloworld

#sudo ip netns delete namespace名
sudo ip netns delete helloworld

# これで作成/削除したnamespaceが見れる
ip netns show 
```

---
---

## network namespaceでコマンドを実行する

```bash
# ip netns exec "namespace"  "command"
ip netns exec helloworld bash
ip netns exec helloworld ip addr show
```

---
---

## 作成したnamespace間を繋げる

namespace間は独立したネットワークなので、通信するには仮想的なLanをnamespaceに繋ぐ必要がある。
画像のように作成したネットワークに属した仮想ethを作成し、ネットワーク間で繋げるイメージ

![VLan2Vlan](png/vlan2vlan.png)

```bash
ip netns add ns1
ip netns add ns2
```

- 仮想NICを作成する

```bash
# veth(仮想NICを作成する)
# ns1-veth0 / ns2-veth0 の仮想NICが作成される。(1対1で繋がってるイメージ？)
ip link add ns1-veth0 type veth peer name ns2-veth0

# vethを確認(作成したnic間で繋がっていることを確認できる)
ip link show 
ip link show | grep veth
```

- 仮想NICをネームスペースに接続する

```bash
# 仮想NICをネームスペースに繋げる
ip link set ns1-veth0 netns ns1
ip link set ns2-veth0 netns ns2

# 仮想NICにIPを割り当てる(*まだstateがdownとなっているので通信はできない)
ip netns exec ns1 ip address add 192.0.2.1/24 dev ns1-veth0
ip netns exec ns2 ip address add 192.0.2.2/24 dev ns2-veth0

# 以下で確認できる
ip netns exec ns1 ip link show ns1-veth0 | grep state
ip netns exec ns2 ip link show ns2-veth0 | grep state
```

```bash
ip netns exec ns1 ip link set ns1-veth0 up
ip netns exec ns2 ip link set ns2-veth0 up
```
