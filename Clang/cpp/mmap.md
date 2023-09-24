# mmapについてまとめる

mmapはシステム系でよく見るので使い方をまとめておく

## そもそも

mmapとはファイルシステムとメモリー領域をマッピングしてくれる関数
宣言はこんな感じ`mmap`でマッピングして`munmap`でリリースする

```cpp
#include <sys/mman.h>
void *mmap(void * addr , size_t length , int prot , int flags , int fd , off_t offset );
int munmap(void *addr, size_t length);
```

addr: マップ先のアドレス、NULL を指定すると新しい領域、それ以外は mmap がマッピングするアドレスのヒントにする
length: マップしたい領域の長さ
prot:マップの保護レベル
flags:変更した結果が他のプロセスから見えるかを指定（ mmap の設定パラメーターも指定可能）
fd:ファイル記述子
offset: 開始点となるファイルオフセット（ファイルマッピングの場合）。ページサイズの倍数とする制限がある

使い方を以下にまとめてみる
