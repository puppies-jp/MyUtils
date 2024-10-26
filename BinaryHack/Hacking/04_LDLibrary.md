# LD系機能についてまとめる

LD系の環境変数は覚えておくと楽なのでメモっておく

- [共有ライブラリを差し替える(LD_PRELOAD)](#PRELOAD)
- [ld-linux.soの動作を確認する(LD_DEBUG)](#DEBUG)

## <a name="PRELOAD">共有ライブラリを差し替える(LD_PRELOAD)</a>

- `LD_PRELOAD`を使用することで、プログラム起動前に共有ライブラリを読み込むことができる
  `LD_LIBRARY_PATH`で設定したフォルダよりも優先的に読み込むらしい

```bash
//NOTE： 以下のようにすることで、共有ライブラリを指定できる
LD_PRELOAD=./libHoge.so  fuga.exe 
```

## <a name="DEBUG">ld-linux.soの動作を確認する(LD_DEBUG)</a>

1. symbols設定することで、シンボル解決の様子が確認できる

    ```bash
    LD_DEBUG=sysbols  XXX.exe
    ```

2. 他の機能
`man ld-linux.so`で他有用なオプションを確認できる。
全ての出力を見る場合、`LD_DEBUG=all`を実行する

## <a name="AUDIT">監査機能を使う(LD_AUDIT)</a>

ld-linux.soがシンボルをbindしたときやPLTエントリ内の関数が呼び出された時にユーザが登録したコールバックを呼び出せる。
つまり、事前に作成したライブラリで登録するためのコールバックを作成する必要がある。

```bash
//　例
LD_AUDIT=libHoge.so xxx.exe
LD_AUDIT=libHoge.so LD_LIBRARY_PATH=. xxx.exe

```

```cpp
//後日サンプルを作成する。
```
