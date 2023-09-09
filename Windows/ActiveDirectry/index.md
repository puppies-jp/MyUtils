# Active Directryについてまとめる

そもそもActive Directryとは、、、

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

```bat
コマンド:
dsmod group "{グループDN}" -addmbr "{ユーザーDN}"

例:
dsmod group "CN=tests,CN=Users,DC=sample,DC=local" -addmbr "CN=test,CN=Users,DC=sample,DC=local"
```
