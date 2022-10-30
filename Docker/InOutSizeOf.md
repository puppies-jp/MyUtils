# Docker in Docker/Docker outside of Dockerについて

## Docker in Docker(DinD)について

Docker in Dockerはコンテナ内でさらにDockerHostを立ち上げ、内部でコンテナを管理する手法

コンテナからさらにコンテナを立ち上げた場合、
`Dockerホストからは起動の有無を確認できない。`
そのため、コンテナ外から操作もできない

しかし、コンテナ内でさらに`DockerEngine`が動くという`オーバーヘッド`が発生する。

1. コンテナを起動する

    ```sh
    # DockerEngineが動くimageで動かす
    docker run -it --rm --privileged --name dind -d docker:20-dind

    # よくあるimageで普通に起動(特権モードで)
    docker run -d --privileged --name dind centos:7 /sbin/init    
    docker exec -it dind bash
    ```

2. コンテナにDocker環境を整える(DockerEngineのインストール)
    以下、環境ごとに整えれば動くようになるはず。  
    (🌟既存でDockerEngineがinstallされている場合は不要)

    1. CentOS

        ```sh
        sudo yum install -y yum-utils
        sudo yum-config-manager \
        --add-repo \
        https://download.docker.com/linux/centos/docker-ce.repo

        sudo yum install docker-ce \
        docker-ce-cli containerd.io \
        docker-compose-plugin

        systemctl start docker
        systemctl enable docker
        ```

    1. Ubuntu

        ```sh
        sudo apt-get update
        sudo apt-get install docker-ce docker-ce-cli containerd.io docker-compose-plugin

        systemctl start docker
        systemctl enable docker
        ```

## Docker outside of Docker

DockerHost上の`docker.sock`というソケットファイルを共有することで
コンテナからDockerHostのDockerEngineを操作する方法。

DockerHost上の操作と同等の操作をするため、別コンテナの操作もできてしまうが、DINDのオーバーヘッドを消せる。

1. Sockファイルを確認する

    ```sh
    # 🌟 macはここにsocketファイルがある。(Linuxも同じみたい)
    user@macbook Docker % ls /var/run/docker.sock 
    /var/run/docker.sock
    ```

2. docker.sockファイルをマウントしてコンテナを起動

    ```sh
    # マウントさせるだけ
    docker run -it --rm -v /var/run/docker.sock:/var/run/docker.sock --name dood docker:20

    # 他imageも同様
    docker run -d -v /var/run/docker.sock:/var/run/docker.sock --name dood centos:7 /sbin/init
    ```

3. Dockerの環境を整える
    DinDと違いコンテナ内で`dockerデーモンを起動するわけではない`ので  
    `インストールするのはCLI`のみでOK

    1. CentOS

        ```sh
        yum install -y yum-utils
        yum-config-manager \
        --add-repo \
        https://download.docker.com/linux/centos/docker-ce.repo

        yum install docker-ce-cli
        ```

    2. Ubuntu

        ```sh
        apt-get install docker-ce-cli
        ```
