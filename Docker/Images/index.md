# Docker Image一覧

便利、よく使うimage をまとめる

```sh
docker pull tensorflow/tensorflow:latest  # Download latest stable image
docker run -it -d --privileged --name tensorJupyter -p 8888:8888 tensorflow/tensorflow:latest-jupyter 

# NOTE 後日、GPUを使う方法もまとめて追記する(一応、Nvidia-containor- toolkitが必要らしい)
https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html

# jupyterでパスワードを設定する方法
# 1.コマンドで変更する(再起動で反映されるはず、起動中に反映されなかった)
jupyter notebook password

# 2. dockerのlogからtokenを読み込んで
# ブラウザ経由でパスワードを変更する
```

## MySQL Server コンテナ

MySQL ServerをDockerコンテナで実行する方法をまとめる

[MySQLコンテナ(2026/5/7)](../src/MySQL/index.md)

## PostgreSQL コンテナ

[PostgreSQL(2026/7/1)](../src/Postgre/index.md)
