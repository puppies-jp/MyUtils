# RestAPI

- [sampleソース](https://github.com/puppies-jp/MyUtils/tree/gh-pages/Python/RestAPI)

```dockerfile
FROM ubuntu:latest
# RUN: docker buildする時に実行される
# Flaskをインストール
RUN apt update && \
    apt install -y openssh-server vim pip && \
    pip install flask

# コンテナの起動時にRestAPIサーバを再起動するようにする
COPY ./main.py /root/
CMD ["/usr/bin/python3", "/root/main.py"]
```

```python
from flask import Flask, jsonify
from flask import make_response
import subprocess as sub

# Flaskインスタンス作成
api = Flask(__name__)

# APIのパス設定
@api.route('/', methods=['GET'])
def rootFunc():
    name = sub.run("hostname",
                   capture_output=True,
                   text=True).stdout
    hello = {
        "message": name.split()[0]
    }

    # レスポンス用のインスタンス生成
    resp = make_response(jsonify(hello), 200)
    resp.headers['Content-Type'] = "application/json"
    return resp

if __name__ == '__main__':
    # 🌟サーバを立ち上げるのはここ！
    api.run(host='localhost', port=8080)
```

使い方

```sh
docker build -t restimg .
```

```sh
# REST API
docker run --rm --name restapi -p 20080:80 restimg

docker service create --name myRest --publish target=80,published=80 \
--replicas=2 --network myNetwork restimg

# curlコマンド
curl -X GET  http://localhost:80/ -v
watch -n 1 curl -X GET  http://localhost:80/ -v
```
