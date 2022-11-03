# RestAPI

- [sampleソース](https://github.com/puppies-jp/MyUtils/tree/gh-pages/Python/RestAPI)

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
