# net コマンドについて

`net`コマンドを使うことで色々なネットワーク周りについて調査、設定をすることができる。覚えると便利なのでまとめる。
使うたびその都度書いていく。
**※ 実行は管理者権限で実行してください**

- [ネットワーク設定コマンド](#network)
- [MTUサイズを確認/変更する](#mtu)
- [net share で共有フォルダを設定する](#share)
- [net use で共有フォルダをドライブに割り当てる](#use)
- [他後日使用の都度追加予定の機能](#etc)

---
---

## <a name=network>ネットワーク設定コマンド</a>

- 外部向けIP設定

```powershell
# 🌟 ファイアウォール有効
netsh advfirewall set allprofiles state on

# 🌟固定IPアドレス/デフォルトゲートウェイ有り
netsh interface ip set address "ローカル エリア接続" static 192.168.xxx.xxx 255.255.255.0 192.168.yyy.yyy
# 🌟2番目のIPアドレスを設定する場合
netsh interface ip add address name="ローカル エリア接続" addr=10.0.0.1 255.255.255.224
# 🌟追加したIPを削除する
netsh interface ip delete address name="ローカル エリア接続" addr=10.0.0.1 mask=255.255.255.224

# 🌟DNS 指定有り
netsh interface ip set dns "ローカル エリア接続" static 192.168.aaa.aaa primary validate=no
netsh interface ip set dns "ローカル エリア接続" static 192.168.bbb.bbb validate=no

# 🌟NIC名称変更
netsh interface set interface name="旧名称" newname="新名称"

# 🌟DHCPにする場合
netsh interface ip set address "ローカル エリア接続" dhcp

# 🌟VLANIDの設定方法
netsh bridge set vlanid "接続名" "VLAN ID"
```

- 内部向けIP設定

```powershell
# 固定IPアドレス/デフォルトゲートウェイ無し
netsh interface ip set address "ローカル エリア接続" static 192.168.xxx.xxx 255.255.255.0 none
# 🌟DNS 無効
netsh interface ip set dns "ローカル エリア接続" none
# ファイアウォール無効
netsh advfirewall set allprofiles state off
```

- 設定内容確認

```powershell
# 🌟NIC名確認方法
netsh interface show interface
# 🌟IP確認方法
netsh interface ip show address "ローカル エリア接続"
netsh interface ip show dns "ローカル エリア接続"
```

- 設定の保存/復旧

```powershell
# 🌟これで設定をダンプしてファイルに吐き出せる
netsh -c "interface ip" dump > xxxxxx.nsh

# 🌟リストアするときはこれ
netsh -f xxxxxx.nsh
```

- ネットワーク確認プロンプト

```powershell
# 接続ポートを確認
netstat -aon
netstat /a  # すべて表示、待ち受けのポートも表示

# MACアドレスキャッシュ一覧
arp -a

# ホスト名 ⇆ IPアドレス
nslookup {hostname}
nbtstat -A {IP address}
```

---
---

## <a name=mtu>MTUサイズを確認/変更する</a>

```bat
# 現在のMTUサイズ、インターフェースID,インターフェース名などをチェックする
netsh interface ipv4 show interface

# idを指定してMTUサイズを変更する
netsh interface ipv4 set interface <interfaceId> mtu=<mtu size>

# MTU変更の疎通確認(MTUサイズ=1500の場合)
:: 1473+28 = 1500 byte
ping -f -l 1472 -n 1 <IP>

:: 1473+28 = 1501 byte
ping -f -l 1473 -n 1 <IP>

|ping指定長       |
|IP      (20byte)|
|Ethernet( 8byte)|
```

---
---

## <a name=share >net share で共有フォルダを設定する</a>

共有リソースの公開／公開停止。ローカルのリソースを公開して、外部のマシンでnet useできるようにする

```bat
:: 🌟共有フォルダの設定を確認する(共有名指定で詳細を確認できる。なくてもOK)
net share [共有名]

:: 🌟作成(デフォルトでこんな感じ)
:: /GRANTのユーザ名は ユーザ名,Everyoneが選べる。
::  アクセス許可レベルは　 READ,CHANGE,FULL　の3つから選択
::  /GRANTオプションは　1回で複数個つけることもできる。
net share 共有名=パス /GRANT:<ユーザ名>,<アクセス許可レベル>  /REMARKS:"コメント"

:: 🌟変更
:: 共有名に作成時と同じオプションをつけるだけ。
:: もしかして、共有名=パスで新しいパスに上書きできたりする？
:: /UNLIMITED -> 同時接続ユーザの制限なし
:: /USERS:ユーザー数 -> 同時接続可能な最大数を指定
net share 共有名 /REMARKS:"コメント" /UNLIMITED

:: 🌟削除
net share 共有名 /Delete 
```

---
---

## <a name=use>net use で共有フォルダをドライブに割り当てる</a>

共有リソースの使用／解除。net shareされたネットワーク上のリソースをローカルで使用する場合に使うコマンド
🚨ネットワーク断線してアクセスするとエラーとなり、復旧後再度コマンドを実行することで再接続できるらしい

```bat
:: 🌟確認 使用中のドライブを確認できる。ドライブ名指定で詳細の確認もできる。
net use [ドライブ名]

:: 🌟作成 共有フォルダをドライブに割り当てる
:: ユーザ名、パスワードが必要な場合, /user:[<ドメイン名\>ユーザ名]　パスワード　で接続できる。
:: ドライブレターがない場合、共有フォルダとして接続される
:: /SAVECRED ユーザとパスワードを保存する オプション
net use <ドライブレター> <\\サーバー名\フォルダ名> /user:ドメイン名\ユーザID パスワード
net use X: ¥¥192.168.1.10¥Share

:: 割り当てたドライブの削除
net use <ドライブレター> /delete
```

---
---

## <a name=etc>他</a>

- ファイル／プリンタ／共有サービス関連
  - net view リソースが公開されているマシンの列挙や、特定のマシンが公開している共有リソースの一覧を調べる
  - net file 使用されているファイルの一覧の表示／強制終了。net shareで公開したリソースのうち、どのようなファイルが実際に外部マシンで利用されているかを表示する
  - net session ローカルのマシンに接続しているクライアントの一覧情報など表示／強制終了

- ユーザー／グループ／コンピュータ・アカウント関連
  - net user ローカル・コンピュータもしくはドメイン・コントローラ上に登録されているユーザー・アカウントに関する情報の表示／設定
  - net accounts ユーザー・アカウントに対するログオンやパスワードの要件の表示／設定
  - net group ローカル・コンピュータもしくはドメイン・コントローラ上に登録されているグループ・アカウントに関する情報の表示／設定
  - net localgroup ローカル・グループ・アカウントに関する情報の表示／設定
  - net computer コンピュータ・アカウントのドメインへの登録／解除

- サービス関連
  - net start  サービスの表示／開始
  - net stop  サービスの停止
  - net pause  サービスの一時停止
  - net continue  サービスの再開

- コンピュータ名／メッセージ関連
  - net name NetBIOS名の表示／追加。新しく追加された名前はnet sendコマンドの宛先として利用できる
  - net send 指定されたユーザーやコンピュータに対するメッセージの送信

- コンフィギュレーション／統計関連
  - net config  サーバ・サービス（リソースの公開サービス）やワークステーション・サービス（リソースの利用サービス）に関する情報の表示／設定
  - net statistics  ネットワーク・プロトコルやリソースの公開／共有サービスに対する統計情報の表示
  - net time  時間情報の表示や外部との同期

- ヘルプ関連
  - net help  各コマンドの使い方の表示
  - net helpmsg  Windowsのエラー番号に対する詳しいエラーメッセージの表示
