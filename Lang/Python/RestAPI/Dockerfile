# どのイメージを基にするか
FROM ubuntu:latest
# RUN: docker buildする時に実行される
RUN apt update && \
    apt install -y openssh-server vim pip && \
    pip install flask

# コンテナの起動時にsshdを再起動するようにする
COPY ./main.py /root/
CMD ["/usr/bin/python3", "/root/main.py"]

#docker service create --name myRest --publish target=80,published=80 \
#    --replicas=2 --network myNetwork restimg
# docker service rm myRest
