# CLコマンドについて

clについてまとめる

[dllの作り方](#dll)
[dllをリンクする](#link_dll)

## <a name=dll>dllの作り方</a>

```c
/* 🌟 __declspec(dllexport)がついてないと
   関数はDLL外部からは見えない */
__declspec(dllexport) int sub(int arg){return 0}
```

```shell
# 🌟cl単体でDLLと.libを生成する方法。
cl ＜ソース＞ /LD

# 🌟linkの場合は/DLLオプション
link /DLL ＜ソース＞
```

- リンクの仕方

```c
// 🌟暗黙的にロードする場合
__declspec(dllimport) int sub(int);

// 🌟明示的にロードする
#include <windows.h>
/* LoadLibrary(dllをロードする) ,
    GetProcAddress(関数ポインタを取得),
    FreeLibrary(ロードをリリースする)
*/
HINSTANCE hDLL;
int (*func)(int);
if ((hDLL = LoadLibrary("sub.dll")) == NULL) printf("LoadLibrary is failed.\n");
else {
    if ((
        func = (int (*)(int))GetProcAddress(hDLL, "sub"))
         == NULL)
        printf("GetProcAddress is failed.\n");
    else
        printf("main %d\n", func(1));
        FreeLibrary(hDLL);
    }
```

## <a name=link_dll>ライブラリを静的にリンクする</a>

```shell
cl <ソース>.c <ライブラリ>.lib
```
