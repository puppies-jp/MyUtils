# BinaryHacks

## ここでは BinaryHacks を読んだ内容をざっくりとまとめる

### ELF(Executable and Linking Format) とは

- 実行可能バイナリ、オブジェクトファイルのフォーマットを規定したもの。ELF フォーマットのファイルは先頭に ELF ヘッダがあり、その後にプログラムヘッダテーブル、セクションヘッダテーブルがある。

  - ヘッダの構造は elf.h にまとまっている。

- これを使うことで、binary ファイルの詳細が得られる。

### builtin 関数

- gcc には builtin で動作時の状況を知る関数などがある。

```cpp
void *__builtin_return_address(unsigned int LEVEL)
関数のリターンアドレス(関数の呼び出しもとを返す関数。)
```
