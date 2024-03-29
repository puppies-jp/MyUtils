# Docker

\[TODO]:`compose`とかと整理が煩雑になってきてるからそのうち整理する！！

- 大まかな流れを以下にまとめる

  1. Docker Hub からイメージを検索／取得する。(search/pull)
  1. [イメージからコンテナを生成して起動(run)](#run)
  1. [コンテナ内でコマンド実行 (exec)](#exec)
  1. [コンテナを停止 (start/stop)](#start_stop)
  1. [コンテナをイメージ化 (commit)](#commit)
  1. [いらなくなったらイメージ/コンテナを削除 (rmi/rm)](#rm)
  1. [イメージをファイルに出力/読み込みを行う](#save_load)

- その他よく利用するコマンド
  - 取得済みイメージ一覧 (images)
  - 実行中コンテナ一覧 (ps)
  - 停止中も含めた全てのコンテナ一覧 (ps -a)
  - 停止中のコンテナを起動 (start)
  - ホストとコンテナ間でのファイルコピー (cp)

- 他
  - [GPU(nvidiaだけではない)を使う](#gpu)
  - [Dockerfileを作成する](WhatDockerfile)
  - [コンテナ間のネットワークを設定する](dockerNetwork)
  - [マルチコンテナアプリケーションを作成する/DockerCompose](DockerCompose)
  - [Docker in Docker/Docker outside of Dockerについて](InOutSizeOf)
    - Dockerコンテナ内でさらにDockerコンテナを動かす(Docker in Docker)
    - Dockerコンテナ内からDockerHostのDockerを操作する

---

## <a name='gpu'>Dockerでグラフィックカード周りの機能を使う</a>

- `CentOS 7.xの場合、/dev/dri/cardX`などのデバイスファイルをdocker上で共有しコンテナと共有して使用するアイデア

```sh
# docker run \
...
--device=/dev/dri:/dev/dri \  ←描画のデバイスを共有
--device=/dev/input:/dev/input \　←ゲームコントローラーのデバイスを共有
...
ubuntu:mygame01 /bin/bash
```

以下、参考リンク

- [Dockerで3Dゲームを動かす――構築編(IT-media)](https://www.itmedia.co.jp/enterprise/articles/1604/27/news001_3.html)

 ---

## Docker アップグレードコマンド

たまにやると忘れそうなので、メモを残しておく

- mac の場合

```sh
brew upgrade --cask docker
```

---

## UbuntuなどでDockerコマンドをsudoなしの設定にする

```sh
# 🌟docker グループに追加したいユーザがいないことを確認する
getent group docker

# 🌟 dockerグループに対象ユーザを追加する
sudo gpasswd -a <ユーザ名> docker
```

---

## Docker Hub からイメージを検索／取得する

- 検索

```sh
$ docker search [image名]

# 例
$ docker search ubuntsu
NAME                         DESCRIPTION        STARS     OFFICIAL   AUTOMATED
ubuntsu/ubuntu                                  0
takuyatomioka/ubuntsu-test   test               0
nomsho/gcp                   gcloud ubuntsu     0
nomsho/java                  git-java ubuntsu   0
```

- 取得

```docker
$ docker pull [image名]

# 例
$ docker pull ubuntsu/ubuntu
```

- 取得結果を確認する

```sh
$ docker images
REPOSITORY               TAG       IMAGE ID       CREATED             SIZE
docker101tutorial        latest    452eceecfe4f   About an hour ago   28.5MB
alpine/git               latest    c6b70534b534   3 weeks ago         27.4MB
uphy/ubuntu-desktop-jp   latest    3ba68e262397   4 weeks ago         1.34GB
uphy/ubuntu-desktop-jp   18.04     2f92249dc622   3 years ago         908MB
```

## <a name="run">イメージからコンテナを生成して起動(run)</a>

- `コンテナを生成して起動` (あくまで生成してから起動していることに注意すること)

```sh
# このコマンドで起動することで $pwdとdesktopのファイルを共有できる
docker run -d -p 8080:8080 -v $(pwd):/root/Desktop --name ubuntuCont uphy/ubuntu-desktop-jp

# ubuntsuコンテナを作成する
docker run -it -d \
--name <コンテナ名> \
-v ~/Docker/Ubuntsu:/root/ \
--privileged \
ubuntu:latest /bin/bash
```

- `-d` : バックグラウンド実行
  付け忘れて実行するとコマンド入力が行えなくなります。その場合は慌てずに別ターミナルを立ち上げて作業 or 停止からの再起動を行う

- `-p 8080(host側):8080(コンテナ側)` : ホスト側の 8080 番ポートをコンテナ側の 8080 番ポートに転送するように指定(GUI が動くのであれば以下にアクセスして GUI に入れるはず)

  > <http://localhost:8080/>

- `-v $(pwd):/root/Desktop`
  ホストのディレクトリ(`$(pwd)`)をコンテナのディレクトリ(`/root/Desktop`)としてマウントします。(`ホスト側のディレクトリも絶対パスで指定する 必要があります。`)
  `-v` でマウントするものを ボリューム と言います。v は volume の頭文字です。

- `--name ubuntuCont`
  生成するコンテナに名前をつけます。名前をつけずにコンテナ ID を利用することもできますが、指定するのが面倒なので名前をつけた方が作業しやすいです。

- `--privileged`
  Dockerコンテナ上ではデフォルトで`systemctl`が使えない、そこでコンテナを
  `特権モード(privileged)`にすることで、`/sbin/init`が使えるようになる。
  Docker Composeの場合はコンテナの設定に以下を追加するだけ
  > privileged: true

---

## <a name="exec">コンテナ内でコマンド実行 (exec)</a>

```sh
docker exec -it [コンテナ名] [コマンド]

# 例
docker exec -it ubuntuCont /bin/bash
docker exec -it ubuntuCont pwd
```

## <a name="start_stop">コンテナを起動/停止する</a>

image からコンテナを作成した場合はここで起動、停止を行う

```sh
docker start [コンテナ名]
docker stop [コンテナ名]

# 例
docker start ubuntuCont
docker stop ubuntuCont
```

## <a name="commit">コンテナをイメージ化 (commit)</a>

```sh
# イメージ命名規則
#<Docker Hubのユーザ名>/イメージ名:[タグ名]

docker commit [コンテナ名] [イメージ名]
docker commit  ubuntuCont local/ubuntu
```

## <a name="rm">いらなくなったらイメージ/コンテナを削除 (rmi/rm)</a>

```sh
# イメージ削除
docker rmi [イメージ名]
# コンテナ削除
docker rm [コンテナ名]

# 例
docker rmi loc/ubuntu
docker rm loc/ubuntu
```

## <a name="save_load">イメージをファイルに出力/読み込みを行う</a>

```sh
# イメージをファイル出力する
docker save [イメージ名] -o [ファイル名].tar
# saveで出力したイメージを読み込む
docker load -i [ファイル名].tar
```
