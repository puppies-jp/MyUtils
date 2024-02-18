# Gitについて色々書く

GitはGitHub/GitLabだけでなく`共有フォルダ`でも他者と共有することができる。
今回は共有フォルダでの設定方法を記す。
手順は以下で作成できる。

1. [PC上に最初のプロジェクトを作る](#1)
2. [プロジェクトのクローン（ベア）を共有フォルダ上に作る](#2)
3. [プロジェクトにリモートリポジトを設定する](#3)

## <a name=1>1. PC上に最初のプロジェクトを作る</a>

```sh
git init
git commit -m 最初のコミット
```

## <a name=2>2. プロジェクトのクローン（ベア）を共有フォルダ上に作る</a>

```sh
# 🌟共有フォルダ上で実行すること
git clone --bare <initを実行したディレクトリ> <リポジトリ名>.git
```

## <a name=3>3. プロジェクトにリモートリポジトを設定する</a>

```sh
git remote add origin <bareリポジトリパス>/<リポジトリ名>.git
git remote update
git push --set-upstream origin master
```


