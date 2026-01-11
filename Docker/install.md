# Docker install手順

何度か引っかかったので、インストール時の手順をまとめる

[公式インストール手順](https://docs.docker.com/engine/install/ubuntu/)

```bash
# Add Docker's official GPG key:
sudo apt update
sudo apt install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

# Add the repository to Apt sources:
sudo tee /etc/apt/sources.list.d/docker.sources <<EOF
Types: deb
URIs: https://download.docker.com/linux/ubuntu
Suites: $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}")
Components: stable
Signed-By: /etc/apt/keyrings/docker.asc
EOF

sudo apt update
```

```bash
sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

# 
sudo systemctl enable docker
sudo systemctl start docker
```

## sudoを回避する

```bash
# docker グループが存在するか確認する
getent group docker

# userをdockerグループに追加
sudo usermod -aG docker $USER

# ユーザが追加されたか確認する
# (再起動後、sudoが不要となるはず。。。)
getent group docker
```
