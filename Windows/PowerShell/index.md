# PowerShellについてまとめる

- [ps1ファイルの実行ポリシーの変更について](#policy)  
- [変数/型について](#type)
- [ヒアドキュメント](#here)
- [PSによるIP/VLAN設定まとめ](ip_setting)
- [実行結果のファイルなど出力](#output)
- [プロセスの親子関係](#ParentProcess)

配列、マップ等あるが、長くなるため別ページに書く。
参考は以下参照  

- [PowerShell 使い方メモ](https://qiita.com/opengl-8080/items/bb0f5e4f1c7ce045cc57)

拡張子は`XXX.ps1`

ここではPowerShellについての文法等をまとめる
まず、特徴として`PowerShell`で扱われるものは，  
**戻り値も含めてほとんどすべてオブジェクト**です。

また、`パイプ処理`を使うことができます。

```PowerShell
# 例 🌟フォルダ内のファイルを一括でリネームしたい場合
Dir | Rename-Item -NewName {$.name -replace "aaa","bbb"}
```

`PowerShellを書くためには少し慣れが必要`ですので，
普通のユーザーがPowerShellで`メリットを得られることは少ない`かも知れません。

それよりも，`システムを管理している人が，何らかの処理を自動化したいと思っているのであれば，そのような場合にPowerShellはうってつけです。`

---
---

## 開発環境(ISE)について

PowerShell には、開発環境として `ISE （あいす）`というエディタが`標準`で用意されている。

入力補完やデバッガ機能が存在するので、 PowerShell のスクリプトを書くときは、基本 ISE を使用することになると思う。

```powershell
# 単一行コメント

<#
複数行
コメントも可能
#>
```

---
---

## <a name=policy>スクリプトを実行できるようにポリシーを変更する</a>

PowerShellが動かない場合、以下が考えられる。

1. ポリシー設定が不十分
2. powershellのバージョンが古い/インストールが壊れている

`ポリシーを変更する`には、 Set-ExecutionPolicy というコマンドレットを使用する。
RemoteSigned を指定した場合、ローカルで作成されたスクリプトは無条件で実行できるが、ネットワークから入手したスクリプトは署名が無ければ実行できない（Windows Server 2012 R2 はこの設定がデフォルト）。

```powershell
> Set-ExecutionPolicy RemoteSigned
実行ポリシーは、信頼されていないスクリプトからの保護に役立ちます。実行ポリシーを変更すると、about_Execution_Policies
のヘルプ トピックで説明されているセキュリティ上の危険にさらされる可能性があります。実行ポリシーを変更しますか?
[Y] はい(Y)  [N] いいえ(N)  [S] 中断(S)  [?] ヘルプ (既定値は "Y"): y
```

---
---

## <a name=type>変数／型について</a>

```powershell
# 🌟 "$"を先頭につけることで、変数として認識する
$hoge = "Hoge"
echo $hoge

# 🌟 ${<任意の文字列>}として定義することもできる。
${日本語　変数} = "ほげ"
echo ${日本語　変数}
```

- 明示的に型を指定することも可能

```powershell
[string] $str = "hoge"
[int] $num = 100

echo $str.GetType()
# string型であれば、大文字に変換して出力すると言ったことも
# echo $str.ToUpper()

echo $num.GetType()
```

```powershell
# 🌟 　出力結果
IsPublic IsSerial Name                                     BaseType            
-------- -------- ----                                     --------            
True     True     String                                   System.Object       
True     True     Int32                                    System.ValueType
```

- 文字列リテラルとエスケープシーケンス(`)
  - エスケープシーケンスは **バッククォート `** である。
  
```powershell
$a = "aaa"

echo "hoge $a" #🌟変数の展開が行われる
# [出力] hoge aaa
echo 'fuga $a' #🌟展開が行われない。
# [出力] fuga $a

$hoge = "HOGE"

# 🚨 変数の展開について
echo "${hoge}_FUGA PIYO" # 🌟${hoge}で展開される
echo "$hoge_FUGA PIYO" # 🌟hoge_Fugaまでが変数と認識
```

---
---

## <a name=here>ヒアドキュメント</a>

```powershell
$fuga = "FUGA"

# 🌟 @" ~~ "@で括るだけ
echo @"
hoge
  $fuga
piyo
"@
```

---
---

## <a name=output>実行結果のファイル出力</a>

- ファイル出力

```ps1
# | からのOut-File or > でOKらしい
Get-Mailbox -Identity User01 |Out-File C:\temp\User01.txt
Get-Mailbox -Identity User01 > C:\temp\User01.txt
```

- CSV出力(Export-Csv)

  - `-Encoding` エンコード指定
  - `-NoTypeInformation` 先頭行に型の情報を入れない

```ps1
Get-Mailbox -Identity User01 | Export-Csv -Path C:\temp\User01.csv -Encoding UTF8 -NoTypeInformation

$Output = Get-Mailbox -Identity User01
$Output += Get-Mailbox -Identity User02
$Output | Export-Csv -Path C:\temp\User.csv -Encoding UTF8 -NoTypeInformation

```

- XMLファイル出力 (Export-CliXml)

コマンドの結果をXMLファイルに出力します。
ひとつのオブジェクトの情報を出力するときによく使います。

設定変更する前の状態をXmlファイルに出力しておけば、
変更前の値を簡単に参照したり、そこから簡単に戻したりできます。

```ps1
# 🌟出力はこれ
Get-Mailbox -Identity User01 | Export-CliXml -Path C:\temp\User01.xml

# 🌟インポートすることでオブジェクトとして扱うことができます。
$File = Import-CliXml -Path C:\temp\User01.xml
```

- Clipボードコピー

```ps1
Get-Mailbox -Identity User01 | Clip
```

---
---

## <a name=ParentProcess>プロセスの親子関係</a>

- chatGPT曰く以下で親プロセスが確認できるらしい

```ps1
Get-WmiObject -Class Win32_Process -Filter "Name='powershell.exe'" |\
 Select-Object ProcessId,ParentProcessId,CommandLine

```
