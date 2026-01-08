# 初期時の設定 tips

```bash
# 日本語のホームフォルダを英語にする
LANG=C xdg-user-dirs-update --force
```

## パッケージインストールでおかしくなったら

aptで以下の様なエラーでパッケージがおかしくなった場合

```bash
libdvd-pkg: Checking orig.tar integrity
/usr/src/libdvd-pkg/libdvdcss_1.4.2.orig.tar.bz2: OK
libdvd-pkg: `apt-get check` failed, you may have broken packages. Aborting
```

これで治るらしい。

```bash
# reconfigure package
sudo dpkg-reconfigure libdvd-pkg
# fix broken package
sudo apt-get install -f
```

## Nvidia Driver

インストール後よくおかしくなるので、まとめる

```bash
#TODO:後で書く、メモ
nvidia-driver-580 
```
