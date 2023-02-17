# AWSについてまとめる

awsについてまとめるためのメモ

[コントロールプレーン](#control)  
[データプレーン](#data)  
[AWS Lambda/AWS Runner について](#lambda)  
    - [EC2/Lambda入門](Lambda)

[AWS S3](S3backet)

- オーケストレータ
  - [ECS(Amazon Elastic Container Service)について](#ECS)  
  - [EKS(Amazon Elastic Kubernetes Service)について](#EKS)

---
---

## オーケストレータについて

コンテナに関して`オーケストレータ`がある。`オーケストレータ`は`処理の分散`、`自動復旧`
`コンテナのデプロイ`機能を担っている。

`Kubernetes`が事実上のデファクトスタンダードであり、他にRed Hatの`OpenShift`
dockerの`swarm`があげられる。
また、`Kubernetes`をベースとした`Amazon EKS`,`GKE(Google)`,`AKS(Microsoft Azure)`などがあげられる。

一般企業が採用する場合、`Kubernetes`はOSSのため、`自身で対処法` or `修正を待つ`
こととなってしまう。  
そういった意味で`OpenShift`であれば技術的なサポートを受けることができる。

---
---

## <a name=control>コントロールプレーン</a>

一般的にIT業界では`「ネットワークのルーティングを制御するモジュール」`であるが、
AWSにおいては`「コンテナを管理する機能」`を指している。

AWSでは`Amazon Elastic Container Service(ECS)`,`Amazon Elastic Kubernetes Service(EKS)`の2つが提供されている。

- ECS  
    AWSが主体のコンテナオーケストレータ 一概にAWSなら`ECS`とは言い切れないが、  
    他のAWSサービスとの連携が容易で、信頼性も高いらしい

- EKS  
    フルマネージドな`Kubernetes`のサービス

---
---

## <a name=ECS>ECSについて</a>

ECS上でアプリを起動するにはコンテナがいる。
コンテナが動作するコンポーネントをECSで`タスク`と呼ぶ。

タスク定義はJSONで行い、イメージ、リソースなどの設定を行うなう。
タスク定義に含めるコンテナ定義は複数設定可

`サービス(Service)` 指定した数だけタスクを維持するスケジューラ。作成時に起動するタスク数、ロードバランサ、タスクを実行するネットワークを指定する
タスクが何らかの理由で終了した場合、タスク定義を元に新しいタスクを生成してタスク数を維持する。

## <a name=EKS>EKSについて</a>

Kubernetesをベースに作られるため、特別`ECS`と違ってAWS独自の用語等を意識することはない。

---
---

## <a name=data>データプレーン</a>

コンテナが実際に稼働するリソース環境。AWSが提供するデータプレーンは2種

- Amazon Elastic Compute Cloud(EC2)  
    EC2は`AWS`で仮想マシンを利用できるサービス、`コア数`,`メモリ容量`,`ストレージ`を可変にでき`ECS`,`EKS`を動かすコンテナのデータプレーンとしても利用される。
    ただし、`EC2`をコンテナホストとして利用するにはOS更新、セキュリティパッチ適用、サーバ監視等が必要となる

- AWS Fargate
    `Fragate`はECS,EKS両方で動作するコンテナ向けサーバレスコンピューティングエンジンで単体利用ができず、`ECS`,`EKS`とセットで利用される

    メリットとして、運用保守がなく、コンテナが実行されるインフラはAWSで最新の状態が保たれる。
    しかし、価格が高く、またAWS側がコンテナの稼働するOSを管理するためリソースのチューニングに不向きとなる
    ビジネスにおいてOSの保守コストとトレードオフでEC2,Fargateを検討する必要がある。

## <a name=anywhere>ECS/EKS Anywhereについて</a>

ECS AnywhereはECSの`コントロールプレーンをAWSで動作`させつつ、
`データプレーンを自身が管理するサーバで動作させる`ことができるサービス

EKS Anywhereはコントロールプレーン、データプレーンを自身が管理するサーバ上で立ち上げるサービス

---
---

## <a name=lambda>AWS Lambda/AWS App Runner について</a>

`lambda`は利用者がコードをアップロードするだけでコードを実行できるサービス。
AWS側でリソースを構築する(Firecrackerと呼ばれるコンテナ上で実行される。)
データプレーンを意識しないで利用できるが、
ECS,EKSといったコントロールプレーンには配置できない

`App Runner`はプロダクションレベルでスケール可能なWebアプリを展開するためのサービス。
GitHubと連携して`ビルド&デプロイ`,ECR(Amazon Elastic Container Registry)のコンテナのイメージデプロイも可能。
ECSとFragateにるアプリの構築と違いネットワーク、ロードバランシング、CI/CD設定の足回りを整える必要がない
