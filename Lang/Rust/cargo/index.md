# Cargoの使い方をまとめる

[公式ドキュメント](https://doc.rust-lang.org/cargo/getting-started/first-steps.html)

## プロジェクト作成

以下、オプションを付けることで、バイナリとライブラリを切り替えられる  
`--bin` : デフォルトでonなので付けないでOK  
`--lib` : ライブラリ作成用  

```bash
# hello_worldフォルダを作成し、以下の以下のファイルを作る
$ cargo new hello_world
# ライブラリ作成時は以下
$ cargo new hello --lib

$ cd hello_world
$ tree .
.
├── Cargo.toml #依存関係を管理するファイル
└── src
    └── main.rs

1 directory, 2 files
```

- 依存関係パッケージ追加

```bash
cargo add regex
cargo add [options] crate…
cargo add [options] --path path
cargo add [options] --git url [crate…]

# 正規表現
cargo add regex
cargo add rand
```

## ビルド&実行

デフォルトで`debug`でビルドされる。
リリースでビルドするには`-r` or `--release`オプションをつけること

```bash
# ビルド
cargo build
cargo build --release
./target/debug/hello_world

# ビルドして実行
cargo run
```
