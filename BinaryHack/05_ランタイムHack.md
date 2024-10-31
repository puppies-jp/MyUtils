# ランタイム Hack

- ここでは Binnary Hack で読んだランタイム Hack についてまとめる

1. [プログラムが main()に辿り着くまで](#start)
2. [共有ライブラリを差し替える(LD_PRELOAD)](#PRELOAD)
3. [trace 機能](#trace)
4. [プログラムの起動高速化](#prelink)
5. [実行中のプロセスにパッチを当てる](#livepatch)

6. [特定のプロセスに見せるファイルを置き換える](#bindmount)

---

## <a name="start">プログラムが main()に辿り着くまで</a>

- まず、Linux において通常のプログラムが main 関数に辿り着くまで

## <a name="PRELOAD">共有ライブラリを差し替える(LD_PRELOAD)</a>

[LD系機能まとめ](./Hacking/04_LDLibrary)

- `LD_PRELOAD`を使用することで、プログラム起動前に共有ライブラリを読み込むことができる

```bash
//NOTE： 以下のようにすることで、共有ライブラリを指定できる
LD_PRELOAD=./libHoge.so  fuga.exe 
```

## <a name="unwind">unwind についてまとめる</a>

- コールチェインを制御する。
  - unwind

## <a name="trace">trace 機能</a>

- システムの trace についてまとめる
  - strace
  - ltrace
  - Jockey

## <a name="prelink">prelink でプログラムの起動を高速化する</a>

## <a name="livepatch">実行中のプロセスにパッチを当てる</a>

- livepatch

## <a name="bindmount">特定のプロセスに見せるファイルを置き換える</a>

以下コマンドで`置き換え先`にアクセスした時に、`置き換え元`にアクセスされる
これとネームスペースを使うことで、特定のプロセスに対してのみ見せるファイルを置き換えられる
(🚨システム全体に反映されることに注意)

```bash
# 特定のファイルを置き換えて見せる
sudo mount -bind "置き換え元ファイルパス" "置き換え先ファイルパス"

# 再起動 or 以下で戻せる
sudo unmount "置き換え先ファイルパス"
```

🌟特定のプロセスでのみ置き換える場合は以下
`unshare`コマンドを用いることで`マウントネームスペース`を作成することができる。

```bash
# 例 以下のようにすることで、$USERに指定してコマンドが実行できる
sudo unshare -m sudo -u $USER /bin/bash 

# さらにバインドで、上記で実行したbash内で置き換えられることになり、bashを抜蹴る際にマウントが解除される。
# 内部的にさらにプロセスを立ち上げても置き換えられた状態が継続する。
sudo mount -bind "置き換え元ファイルパス" "置き換え先ファイルパス"
```
