# ライブラリを使う

ライブラリの使い方を覚えてみる。

## ライブラリを指定して読み込み

- `dlopne`の使い方
  `void* dlopen(ライブラリ名,オプション)`で使用する。
  オプションは`RTLD_LAZY`,`RTLD_NOW`が指定
  - `RTLD_LAZY`:関数の参照の解決を実行時まで遅延させる。ライブラリ内に参照を解決できない関数があってもエラーにならない
  - `RTLD_NOW`:`dlopen`時に参照を解決させる

- `dlsym`の使い方
  `void* dlsym(dlopenのポインタ,"関数名")`で使用する
  dlopneしたポインタの代わりに`RTDL_NEXT`を指定することで自身よりも後にロードされたオブジェクトからシンボルを探す。つまり、元のライブラリ関数を読み出す。

```cpp
//Windows .dll
//MAC  .dylib
//Linux .so

const auto addlib = dlopen("./libSharedAdd.dylib", RTLD_LAZY);
const auto sublib = dlopen("./libSharedSub.dylib", RTLD_LAZY);

auto Add = reinterpret_cast<double(*)(double, double)>(dlsym(addlib, "calculation"));
auto Sub = reinterpret_cast<double(*)(double, double)>(dlsym(sublib, "calculation"));

dlclose(addlib);
dlclose(sublib);
```

```cpp
#include <dlfcn.h>
typedef int (*TPRINTF)(const char *, ...);
const static TPRINTF ORG_PRINTF = (TPRINTF)dlsym(RTLD_NEXT, "printf");

int printf(const char *fmt, ...)
{
    ORG_PRINTF(PREFIX);

    va_list ap;
    // 可変長引数を１個の変数にまとめる
    va_start(ap, fmt);
    // まとめられた変数で処理する
    int ret = vprintf(fmt, ap);
    va_end(ap);
    return ret;
};

```
