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

- サンプル1
    イベントログを取得
    (syslog,applicationログを別々に出力する場合)

```powershell
#任意の出力先フォルダと-afterで指定する時間を設定
$OutPutFolder = Join-Path ([Environment]::GetFolderPath('Desktop')) "EventLogs"
if(!(Test-Path $OutPutFolder)){ mkdir $OutPutFolder }
$yesterday = (Get-Date).Date.AddDays(-1)

#Systemログ、Applicationログを別々に$OutPutFolderへ保存する処理。
@("System", "Application") | %{
    Get-EventLog $_ -After $yesterday |
    ?{ ($_.EntryType -eq "Error") -or ($_.EntryType -eq "0") } |
    Export-Csv -Encoding Default -NoTypeInformation -Path (
        Join-Path $OutPutFolder ($_ + "Log_" + (Get-Date).Date.ToString("yyyyMMdd") + ".csv") #任意の出力ファイル名
    )
}
```

- サンプル2
    イベントログを取得
    (syslog,applicationログを別々に出力する場合)

```powershell
#任意の出力先フォルダと-afterで指定する時間を設定
$OutPutFolder = Join-Path ([Environment]::GetFolderPath('Desktop')) "EventLogs"
if(!(Test-Path $OutPutFolder)){ mkdir $OutPutFolder }
$yesterday = (Get-Date).Date.AddDays(-1)

Get-WinEvent -FilterHashtable @{
    LogName='System', 'Application'
    Level=1,2
    StartTime=$yesterday
} | 
Select-Object -Property * |
Export-Csv -Encoding Default -NoTypeInformation -Path (
        Join-Path $OutPutFolder ("WinEventLog_" + (Get-Date).Date.ToString("yyyyMMdd") + ".csv") #任意の出力ファイル名
    )

```

[Get-WinEvent使い方](https://forsenergy.com/ja-jp/windowspowershellhelp/html/62e7642c-51d4-47d1-97fe-62b08197896a.htm)
[🌟イベントログ -> CSVファイル出力](https://qiita.com/hara_power/items/073fa6079e633f084412)

---

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

- VLANについて(そのうち書く)

    管理者権限で`PowerShell`を起動すること

```powershell
# 🌟IntelNIC用のモジュールをインポート
Import-Module -Name “C:\Program Files\Intel\Wired Networking\IntelNetCmdlets”

# 🌟NICの名前(ParentName)を取得
Get-IntelNetAdapter

# 🌟NICの名前を-ParentName に指定し、追加したいVLANIDを指定
Add-IntelNetVLAN -ParentName “<NIC名>” -VLANID <VLANID>

# example:
Add-IntelNetVLAN -ParentName “Intel(R) I350 Gigabit Network Connection” -VLANID 300
```

```powershell
# 🌟VLAN名前の変更
Set-IntelNetVLAN -ParentName “<NIC名>” -VLANID <VLANID> -NewVLANName “<VLAN名>”

# 🌟VLAN Interfaceの削除
Remove-IntelNetVLAN -ParentName “<NIC名>” -VLANID <VLANID>
```
