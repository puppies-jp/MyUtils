# GitHub系

## GitHubに公開鍵・秘密鍵を設定してssh接続

[Qiita参考](https://qiita.com/shizuma/items/2b2f873a0034839e47ce)

```bash
# 秘密鍵/公開鍵作成
cd ~/.ssh
ssh-keygen -t rsa -f "outputkeyfile"

# これの中身をgithubで設定することで接続できるようになる
"outputkeyfile".pub

# 設定後
ssh -T git@github.com
```

* 公開鍵/秘密鍵をby nameでファイル名を作成した場合、s
  shや`push`でうまく行かないことがある。
  そういう場合、`~/.ssh/config`に以下を追記することで、秘密鍵を使うように設定することができる。

```bash
Host github github.com
  HostName github.com
  IdentityFile ~/.ssh/"ファイル名(秘密鍵)" #ここに自分の鍵のファイル名
  User git
```
