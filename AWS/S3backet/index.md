# AWS S3

AWS S3 について書いてみる

- [minioについて](#minio)

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

