# Docker Image一覧

便利、よく使うimage をまとめる

```sh
docker pull tensorflow/tensorflow:latest  # Download latest stable image
docker run -it -d --privileged --name tensorJupyter -p 8888:8888 tensorflow/tensorflow:latest-jupyter 

# NOTE 後日、GPUを使う方法もまとめて追記する

# jupyterでパスワードを設定する方法
# 1.コマンドで変更する(再起動で反映されるはず、起動中に反映されなかった)
jupyter notebook password

# 2. dockerのlogからtokenを読み込んで
# ブラウザ経由でパスワードを変更する
```

