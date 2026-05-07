# Rust

Rustが話題なので勉強する
ライブラリとしてビルドすることで、C/C++のライブラリとして読み込めるライブラリにもなる。  
(ビルド時のアノテーションが必要)

- [構文](./grammar.md)
- [所有権と参照/借用](OwnerShip)
- [パッケージ作成](./cargo/index.md)
- [他基本構文](./basic/)
- [MySQLクレート](./cargo/mysql/index.md)

---

## 環境構築

- [インストール手順(公式)](https://rust-lang.org/ja/tools/install/)

`rustup`を使って`Rust`をインストールするらしい

```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

`~/.cargo/bin`ディレクトリに基本的なツールがインストールされるので、
以下で環境変数を書き換え

```bash
export PATH=$HOME/.cargo/bin:$PATH
```

## アップデート

以下でOK

```bash
rustup update
```

---
---

## 各種ツール

|ツール|機能|
|--|--|
|rustc|コンパイラ|
|cargo|プロジェクト作成/パッケージマネージャ|
|rustdoc|Rustのソースからドキュメントを作るらしい|

[公式](https://rust-lang.org/ja/learn/)

---

## コンパイル

- 普通に一つのファイルをコンパイルするパターン

```rust
//hello.rs
fn main() {
    println!("Hello, world!");
}
```

```bash
rustc src/hello.rs
#gcc/g++同様 -oオプションで出力名を選べるらしい
rustc src/hello.rs -o hello.out 
./hello # on a *NIX
.\hello.exe # on Windows
```

- 複数ファイルでコンパイルする場合

```rust
// main.rs
mod foo;//fooを読み込むらしい

fn main() {
    foo::hello();
}
```

```rust
// foo.rs
pub fn hello() {
    println!("Hello, world!");
}
```

```bash
# mainから遡って読み込むっぽい
rustc main.rs
```
