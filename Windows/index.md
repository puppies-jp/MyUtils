# Windowsについて

不服だが、仕事上使うのでメモを書いていく

- [イベントログ](#EventLog)  
- [ネットワーク](#network)  

## <a name=EventLog>イベントログについて</a>

`イベントログ`は`イベントログビューア`でも確認できるが、
いちいちアプリを起動するのは鬱陶しい。
バッチを叩いてログを収集したい。
そのためのメモをここに残す。

```powershell
# ログだけ取りたいならこれ。
Get-WinEvent

# イベントログビューアを起動 まあ、いらない  
# /? オプションをつけることでhelpを表示できる
eventvwr.exe
```

[Get-WinEvent使い方](https://forsenergy.com/ja-jp/windowspowershellhelp/html/62e7642c-51d4-47d1-97fe-62b08197896a.htm)
[🌟イベントログ -> CSVファイル出力](https://qiita.com/hara_power/items/073fa6079e633f084412)

## <a name=network>ネットワーク設定バッチ</a>

- 外部向けIP設定

```powershell
# //🌟 ファイアウォール有効
netsh advfirewall set allprofiles state on
# //🌟固定IPアドレス/デフォルトゲートウェイ有り
netsh interface ip set address "ローカル エリア接続" static 192.168.xxx.xxx 255.255.255.0 192.168.yyy.yyy
# //🌟DNS 指定有り
netsh interface ip set dns "ローカル エリア接続" static 192.168.aaa.aaa primary validate=no
netsh interface ip set dns "ローカル エリア接続" static 192.168.bbb.bbb validate=no
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
netsh interface ip show address "ローカル エリア接続"
netsh interface ip show dns "ローカル エリア接続"
```

## ネットワーク確認プロンプト

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

- VLANについて(そのうち書く)
