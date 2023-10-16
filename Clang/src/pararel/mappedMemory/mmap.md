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

- addr: マップ先のアドレス、NULL を指定すると新しい領域、それ以外は mmap がマッピングするアドレスのヒントにする
- length: マップしたい領域の長さ
- prot:マップの保護レベル
- flags:変更した結果が他のプロセスから見えるかを指定（ mmap の設定パラメーターも指定可能）
- fd:ファイル記述子
- offset: 開始点となるファイルオフセット（ファイルマッピングの場合）。ページサイズの倍数とする制限がある

使い方を以下にまとめてみる

```cpp
const unsigned int FILE_LENGTH = 0x100;
const std::string FILE_NAME("./data.dat");

int main()
{
    /* Prepare a file large enough to hold an unsigned integer. */
    auto fd = open(FILE_NAME.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    lseek(fd, FILE_LENGTH + 1, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);
    /* Create the memory mapping. */
    char *const file_memory = 
    reinterpret_cast<char *>(
        mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0)
    );
    // char *const file_memory = reinterpret_cast<char *>(mmap(0, FILE_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    close(fd);
    /* Write a random integer to memory-mapped area. */
    sprintf(file_memory, "%s", "Hello World!!!!");
    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH);

    return EXIT_SUCCESS;
}

```
