# どのイメージを基にするか
FROM ubuntu:latest
# RUN: docker buildする時に実行される
RUN apt update && \
    apt install -y openssh-server vim pip && \
    pip install flask

# コンテナの起動時にsshdを再起動するようにする
COPY ./main.py /root/
#RUN  chmod a+x /root/setup.sh
#CMD ["bash", "/root/setup.sh"]
CMD ["/usr/bin/python3", "/root/main.py"]
###
# docker run --name UbuntsuSsh -p 20022:20022 -e TZ=Asia/Tokyo test/ssh 
#docker run --rm --name RestServer -p 20022:88 \
#-e TZ=Asia/Tokyo 9e05f211c5e1


# docker service create --name myRest --publish target=80,published=80 \
# --replicas=2 --network myNetwork restimg
# docker service rm myRest
