# Kubernetes

デプロイやスケーリングを自動化したり、コンテナ化されたアプリケーションを管理したりするための、オープンソースのシステムです。 管理や検出を容易にするため、アプリケーションを論理的な単位に分割し、コンテナをグルーピングします。

- Googleが開発したコンテナ管理システムで「クバネティス」「クバネテス」「クーべネティス」と呼ばれる。
- コンテナを Pod と呼ばれる単位で複数のノードに配布して実行管理する。
- クライアントからのアクセスを適切にノード上の Pod に転送する仕組みを備えている
- コンテナやノードがダウンしても代わりに他のノードでコンテナを起動してくれる
- サービスを停止することなく、コンテナをローリングアップデートしたりアップデートを中断してロールバックすることができる

- マスターサーバ : コンテナの管理をする。また、外部から管理可能にするためのAPIを提供する

- ノードサーバ : ノードはコンテナのホストをする
  - Pod : コンテナが1つ以上ある箱、Pod毎にIPをもつ

Kubernetes の構築には通常1台のマスターサーバ、3台以上のノードサーバが必要ですが、学習用にに1台のサーバ上で Kubenetes を動かすことができる Minikube が提供されています。

---

## Minikube使い方メモ

- 最新バイナリ取得コマンド

```sh
curl -Lo minikube https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64 
chmod +x minikube

# インストールコマンド
sudo install minikube /usr/local/bin/
```

```sh
# 起動コマンド
minikube start --driver=docker
minikube start

# ステータス確認コマンド
minikube status
```

- dashboard起動

[公式リンク](https://kubernetes.io/ja/docs/tasks/access-application-cluster/web-ui-dashboard/)
[Qiita設定手順](https://qiita.com/dingtianhongjie/items/6e7acc84e95453cd782b)

```sh
minikube dashboard
```
