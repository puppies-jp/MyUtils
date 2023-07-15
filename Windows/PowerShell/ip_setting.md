# PowerShellによるIP設定周りをまとめる

[NIC名称を変更する](#nic)

結論こんな感じ

```ps1
# IP設定方法
Get-NetAdapter -name Wi-Fi| \
New-NetIPAddress -AddressFamily IPv4 -IPAddress 192.168.1.10 -PrefixLength 24 -DefaultGateway 192.168.1.1

# DNS(優先、代替設定するとこんな感じ)
Get-NetAdapter | Set-DnsClientServerAddress -ServerAddresses 192.168.1.5, 192.168.1.6
```

---
---

## <a name=nic>NIC名称を変更する</a>

```ps1
# NIC名を確認する
Get-NetAdapter 
# 🌟Nic名を変更する場合
Rename-NetAdapter -Name "ロール エリア接続" -NewName "LANアダプタ"
```

---
---

## <a name=vlan>VLANを設定する</a>

```ps1
# 🌟IntelNIC用のモジュールをインポート
Import-Module -Name “C:\Program Files\Intel\Wired Networking\IntelNetCmdlets”

# 🌟NICの名前(ParentName)を取得
Get-IntelNetAdapter

# 🌟NICの名前を-ParentName に指定し、追加したいVLANIDを指定
Add-IntelNetVLAN -ParentName “<NIC名>” -VLANID <VLANID>

# example: 
# 複数設定する場合:VLanId1,VLanId2,VLanId3
# 範囲を指定する場合: (VLanId1..VLanId4)            
Add-IntelNetVLAN -ParentName “Intel(R) I350 Gigabit Network Connection” -VLANID 300,400,500
```

- VLAN インタフェースを変更する場合

```powershell
# 🌟VLAN名前の変更
Set-IntelNetVLAN -ParentName “<NIC名>” -VLANID <VLANID> -NewVLANName “<VLAN名>”

# 🌟VLAN Interfaceの削除
Remove-IntelNetVLAN -ParentName “<NIC名>” -VLANID <VLANID>
```
