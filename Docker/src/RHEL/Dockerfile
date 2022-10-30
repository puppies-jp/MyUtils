# どのイメージを基にするか
FROM ubuntu:latest

# RUN: docker buildする時に実行される
#RUN apt update && \
#    apt install -y openssh-server vim \
#    sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config\
#    sed -i 's/#Port 22/Port 20022/' /etc/ssh/sshd_config

# コンテナの起動時にsshdを再起動するようにする
#COPY ./config_files/setup.sh /root/
#RUN  chmod a+x /root/setup.sh
#CMD ["bash", "/root/setup.sh"]
# CMD ["bash"]
###
# docker run --name UbuntsuSsh -p 20022:20022 -e TZ=Asia/Tokyo test/ssh 
