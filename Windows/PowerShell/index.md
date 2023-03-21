# PowerShellについてまとめる

- [ps1ファイルの実行ポリシーの変更について](#policy)  
- [変数/型について](#type)
- [ヒアドキュメント](#here)

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

ポリシーを変更するには、 Set-ExecutionPolicy というコマンドレットを使用する。
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
