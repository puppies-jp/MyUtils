# Dockerfileについて

DockerfileはDockerコンテナのビルド時や起動時の作業を一つの設定ファイルにまとめたもの

- [公式リンク](http://docs.docker.jp/engine/reference/builder.html)

1. dockerfileのビルドコマンドについて

```bash
# ディレクトリ直下のdockerファイルを実行すr
docker build .
# dockerfileを指定して実行する。
docker build -f /path/to/a/Dockerfile .
# ファイル指定かつイメージの[リポジトリ/タグ名]を指定できる。
docker build -f /path/to/a/Dockerfile -t repoName/tagName .

# 他オプションは以下
docker build --help
```

---

## Dockerfileのコマンドについて

### FROM

```dockerfile
# コンテナの元となるイメージ,タグなしの場合、latest(最新)が選択される
FROM イメージ[:<タグ>]
# example
FROM python:3.7-alpine
```

### RUN(ビルド時に実行するコマンド)

```dockerfile
RUN <コマンド>
# example
RUN apt-get update
RUN apt-get install iputils-ping net-tools
```

### CMD(起動時に実行するコマンド )

🚨1つのDockerfileで1度だけ指定できる

```dockerfile
CMD <コマンド>
# example
CMD python -m http.server 80
```

### ENV(環境変数の定義)

```dockerfile
ENV <環境変数名>=<値>
# example
ENV TZ=Asia/Tokyo
# イコールの代わりに半角スペースでもOK
ENV DB_ENDPOINT 172.16.0.2
```

### COPY(ホストやURLからコンテナへファイルをコピー)

- コピー元がディレクトリの場合、ディレクトリ内すべてコピーされる
- コピー先ディレクトリが存在しない場合は作成される

```dockerfile
COPY <コピー元ファイル名(orディレクトリ名)> <コピー先ディレクトリ名>
# example
COPY ../web/src/index.html /var/www/
COPY ../web/script/ /var/www/script/
COPY *.txt /var/www/docs/
```

### WORKDIR(命令を実行するディレクトリを指定)

ディレクトリを指定したいRUN、CMD、COPY等の命令の直前に記述する

```dockerfile
WORKDIR <コンテナ内ディレクトリ名>
# example
WORKDIR /var
RUN mkdir www
WORKDIR /var/www
CMD python -m http.server 80
```

### USER(命令を実行するユーザを指定)

```dockerfile
USER <ユーザ名 or UID>
# example
USER john
```

### userを指定して実行する

```dockerfile
# create group and user
RUN groupadd -r tom && useradd -g tom tom

# set ownership and permission
RUN chown -R tom:tom /app

# switch to user
USER tom

# run with tom
CMD node index.js
```

### マルチステージビルド

- `FROM`を二つ使う。片方のイメージでファイルなどのビルドを行い、二つ目の `FROM`のイメージとファイルを`COPY`することで、build環境を引き継がずに生成物のみを使い回すことができる。
`FROM <image> as <name>`とすることで、タグ付けができる。

```dockerfile
# syntax=docker/dockerfile:1
FROM golang:1.16
WORKDIR /go/src/github.com/alexellis/href-counter/
RUN go get -d -v golang.org/x/net/html
COPY app.go ./
RUN CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .

FROM alpine:latest
RUN apk --no-cache add ca-certificates
WORKDIR /root/
COPY --from=0 /go/src/github.com/alexellis/href-counter/app ./
CMD ["./app"]
```

```dockerfile
# syntax=docker/dockerfile:1
FROM golang:1.16 AS builder
WORKDIR /go/src/github.com/alexellis/href-counter/
RUN go get -d -v golang.org/x/net/html
COPY app.go    ./
RUN CGO_ENABLED=0 GOOS=linux go build -a -installsuffix cgo -o app .

FROM alpine:latest
RUN apk --no-cache add ca-certificates
WORKDIR /root/
COPY --from=builder /go/src/github.com/alexellis/href-counter/app ./
CMD ["./app"]

```
