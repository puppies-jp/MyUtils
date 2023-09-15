# Active Directryについてまとめる

そもそもActive Directryとは、以下らしい。

> 一般的には「ディレクトリサービス」と呼ばれる機能の一種で、ネットワーク上に存在するWindowsで稼働するサーバやクライアントなどのコンピュータ、プリンタなどの周辺機器、それらを使用する利用者の識別・認証情報（ユーザーアカウント）や各資源へのアクセス権限などの情報を一元管理することができる。

Active Directoryでは、**同じ資源情報のデータベースを共有する範囲のことを「ドメイン」（他の分野の用語「ドメイン」と区別したい場合は「ADドメイン」とも）という。**

ドメインにはデータベースの管理を行う「ドメインコントローラ」が置かれ、**ドメイン内の資源や利用者、アクセス権限などについての情報を一元的に登録、管理する。ドメイン内の利用者やクライアント機はドメインコントローラに問い合わせを行い、ログオン手続きや資源へのアクセス可否の判断などを行う。**

Active Directoryにおいて、OUとグループは異なるものです。
グループとOUはまったく別個の存在なので、オブジェクトを配置しているOUとグループのメンバシップは、*まったく連動していない。*
*「グループはアクセス権付与の単位」*、*「OUはグループポリシーの設定や管理委任の単位」*と、完全に頭を切り替える必要がある。

- OUは、組織単位として、ユーザーやコンピューターアカウントを整理して格納するためのフォルダーのようなものであり、グループポリシーを適用する単位。
- グループは、ユーザーやコンピューターアカウントをまとめて管理するためのものであり、アクセス権限の設定やグループポリシーの適用などに使用されます。

ご参考までに、OUの名前を変更する方法については、Microsoft Learnに詳細な手順が記載されています⁵。手順は以下の通りです：

1. Active Directoryユーザーとコンピューターを開きます。
2. 左側のツリー画面で、変更したいOUがある場所（ドメイン、OUまたはコンテナ）を選択します。
3. 右側の一覧で対象となるOUを選択して、[操作]-[名前の変更]を選択します。
4. 名前を変更し、[OK]をクリックします。

---
---

ちなみにDC,CNは以下の略らしい。
んで、端的にDC -> サーバ名,CN -> グループ名って感じ
DC(domain controller)
CN(common name)

## ユーザグループを作成する

```bat
コマンド:
dsadd group "{グループDN}"

例:
dsadd group "CN=tests,CN=Users,DC=sample,DC=local"
```

## ユーザを作成する

```bat
コマンド:
dsadd user "{ユーザーDN}" -display "{表示名}" -pwd "{パスワード}" -pwdneverexpires yes -upn "{ユーザーID}@{ドメイン名}"

例:
dsadd user "CN=test,CN=Users,DC=sample,DC=local" -display "テスト" -pwd "password" -pwdneverexpires yes -upn "test@sample.local"

## こんな感じで追加される
ActiveDirectoryユーザーとコンピューター
|-sample.local
    |-Users
        |-test
```

## ユーザ名をグループに追加する

```bat
コマンド:
dsmod group "{グループDN}" -addmbr "{ユーザーDN}"

例:
dsmod group "CN=tests,CN=Users,DC=sample,DC=local" -addmbr "CN=test,CN=Users,DC=sample,DC=local"
```

## グループ名を変更する

```bat
# 「ad-domain.company.local」ドメインのコンテナ「Users」にあるグループ「Sample1」について、識別名を「Sample2」に変更
dsmove cn=Sample1,cn=Users,dc=ad-domain,dc=company,dc=local -newname Sample2
```

## OU名を変更する

```powershell
Import-Module ActiveDirectory
Set-ADOrganizationalUnit -Identity "旧いOU名" -NewName "新しいOU名"
```

## PowerShellでGPOを操作する

```powershell
Import-Module -Name ActiveDirectory
Import-Module -Name GroupPolicy 

# GPOを作成する
New-GPO -Name "GPO Name Sample" -Comment "This is a test GPO."
# GPOをOUに割り当てる
New-GPLink -Name "GPO Name Sample" -Target "OU=OUName,OU=COMPUTERS,DC=example,DC=local" -Enforced Yes

```
