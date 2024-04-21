# ライブラリを使う

ライブラリの使い方を覚えてみる。

## ライブラリを指定して読み込み

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
