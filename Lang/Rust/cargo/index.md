# Cargoの使い方をまとめる

[公式ドキュメント](https://doc.rust-lang.org/cargo/getting-started/first-steps.html)

## プロジェクト作成

以下、オプションを付けることで、バイナリとライブラリを切り替えられる
`--bin` : デフォルトでonなので付けないでOK
`--lib` : ライブラリ作成用

```bash
# hello_worldフォルダを作成し、以下の以下のファイルを作る
$ cargo new hello_world

$ cd hello_world
$ tree .
.
├── Cargo.toml
└── src
    └── main.rs

1 directory, 2 files
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
