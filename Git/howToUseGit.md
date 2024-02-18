# Git使い方 備忘録

```sh
# リポジトリからファイルを取得する。
git clone  <リポジトリ>

```

- ローカルbranch表示

```sh
# ブランチ表示(cloneしたローカル環境にあるブランチのみ表示される)
git branch
# ブランチ作成
git branch <branch名>

# ブランチ切替 (checkout)
git checkout <branch名>
git checkout -b <branch名>　# ブランチ切替、作成もやってくれる
```

## マージ(merge)

`branch1(マージ元)`の変更を`master(マージ先)`へ取り込む場合

```sh
# 🌟branch1の変更がコミット済、プッシュ済の前提
# 1.編集中のブランチをマージ先に切り替える
git checkout master

# 2.マージ元のブランチを指定してマージを行う
git merge branch1


# 3. コンフリクトを起こした場合(merge modeとなる)
## 3.1.1 コンフリクトを解消してadd,commitする
git status  # コンフリクトしたファイルを確認できる
git add <対象のファイル>  # addすることで、コンフリクトを解消した印となる

## 3.1.2 全てのコンフリクトを解消したらコミットでマージが確定する
git commit 

## 3.2 マージを取り下げる場合
git merge --abort

# 4.マージ結果をプッシュする
git push
```

## リベース(rebase)
