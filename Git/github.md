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