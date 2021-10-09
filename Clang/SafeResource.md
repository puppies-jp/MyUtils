# C++ デストラクタによる安全なリソース解放について

- ファイルの close 忘れ、接続のクローズ、メモリの free 忘れはよくやる。。。(うっかりとか色々な理由で、)

  - 忘れるだけならまだ大丈夫だが、例外がスローされ close を呼ばずに次の処理に移ると手が追えなくなってしまう。

- 上記の問題に対して以下のように実装し、デストラクタを使うことでスコープを抜ける、delete するタイミングでデストラクタ内で処理することで安全に資源を確保することができる。

- header ファイル

```cpp
 //
 // FileHandle.h
 //
 #ifndef __FileHandle_H__
 #define __FileHandle_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class FileHandle {
public:
    // public member functions
    // 🌟コンストラクタ(ここでfileをopenする。)
    FileHandle(const char *filepath);
    // 🌟デストラクタ(ここでcloseする。)
    virtual ~FileHandle();

    // 🌟int として参照されるとき呼ばれるメソッド
    virtual operator int(void);
protected:
    // member variable
    // 🌟ファイルディスクリプタを保存するメンバ変数
    int fd;
};

#endif
```

- cpp の実装

```cpp
//🌟コンストラクタ
FileHandle::FileHandle(const char *filepath)
{
    fd = safeopen(filepath);
    //ファイルディスクリプタを保存
}
// デストラクタ
FileHandle::~FileHandle()
{
    // ファイルディスクリプタが-1でない時、closeする。
    // (safeopenで失敗した時 -1を返す)
    if(fd!=-1) close(fd);

    /* 🌟　以下のような使い方ができる。
    FileHandle fh("Test.txt");  ← FileHandle クラスのインスタンスを生成
    if(fh==-1) {
        printf("file open error\n");
        return;
    }
    */
}

// int型で参照した場合のop関数
FileHandle::operator int(void)
{
    // ファイルディスクリプタをそのまま返す。
    return fd;
}

static int safeopen(const char *filepath)
// 🌟安全にファイルをオープンする関数
{
    struct stat lst, fst;
    int fd;

    if (lstat(filepath, &lst) != 0) return -1;
    if (!S_ISREG(lst.st_mode)) return -1;
    fd = open(filepath, O_RDWR, 0);

    if (fd < 0) return -1;

    if (fstat(fd, &fst) != 0) {
       close(fd);
       return -1;
    }

    if (lst.st_ino != fst.st_ino || lst.st_dev != fst.st_dev) {
       close(fd);
       return -1;
    }
    return fd;
}
```

- 参考: [ファイルの open から close を安全に行うための仕組み](https://www.ipa.go.jp/security/awareness/vendor/programmingv1/b06_06_main.html)
