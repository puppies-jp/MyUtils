# AWS S3

AWS S3 について書いてみる

- [minioについて](#minio)
- [hostingする](#hosting)
- [lambdaでアクセスする](#lambda)

## <a name=minio>minio を使ってみる</a>

`minio`とは、`s3バケット`をローカル環境で模擬的に動かすコンテナ

```sh
mkdir -p ./minio/data

docker run \
-p 9000:9000 \
-p 9090:9090 \
--name minio \
-v $(pwd)/minio/data:/data \
-e "MINIO_ROOT_USER=ROOTNAME" \
-e "MINIO_ROOT_PASSWORD=CHANGEME123" \
quay.io/minio/minio server /data --console-address ":9090"
```

## <a name=hosting>hostingについて</a>

S3は格納したhtmlから静的Webページを作成できる。やり方は以下参照。
基本的にファイルを格納してアクセス権限を付与すればOK

[Amazon S3 を使用して静的ウェブサイトをホスティングする](https://docs.aws.amazon.com/ja_jp/AmazonS3/latest/userguide/WebsiteHosting.html)

## <a name=lambda>LambdaからS3へアクセスする</a>

python3なら`boto3`というライブラリを経由してファイルにアクセスできる。
権限の設定として
`lambdaでバケットのアクセス権を付与する` or `バケットに権限をアクセス権を付与する` の2通りがある。

```python
import json
import time
from datetime import datetime

# boto3を使うことでAWSのバケットの操作を行う
import boto3

# clientインスタンスを作成(引数は普通に's3'でOK)
# 他の使い方は知らん
s3 = boto3.client('s3')

def lambda_handler(event, context):
    # TODO implement
    print(context)
    
    bucket = '<バケット名>'
    key = '<バケットのファイルパス>'

    # バケットとファイルを指定することで取ってきてくれる
    # dir型の他の中身は知らん
    response = s3.get_object(Bucket=bucket, Key=key)
    body = response['Body'].read()

    return {
        'statusCode': 200,
        'body': body,
        'event':event # 第一引数eventはcurlの -d '{}'で指定できる
    }
```
