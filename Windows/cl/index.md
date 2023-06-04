# CLコマンドについて

clについてまとめる

[dllの作り方](#dll)
[dllをリンクする](#link_dll)

## <a name=dll>dllの作り方</a>

```cpp
#ifdef EXPORTS // ライブラリ定義側用
#define DECLSPEC __declspec(dllexport)
#else // ライブラリ呼び出し側用
#define DECLSPEC __declspec(dllimport)
#endif

void DECLSPEC hello();
```

```cpp
//🌟 上記の関数プロトタイプ宣言を dllexport に倒すこと
#define EXPORTS
#include <~~~>

//関数の定義
void hello(){
   printf("hello\n");
}
```

```c
/* 🌟 __declspec(dllexport)がついてないと
   関数はDLL外部からは見えない */
__declspec(dllexport) int sub(int arg){return 0}
```

```shell
# /D プリプロセッサ シンボル定義 (/DDEBUG ,/DTEST=1,etc..)
# /Z7,/Zi,/ZI デバッグ系コンパイラ オプション 
    # デバッグ情報をオブジェクト ファイルに保存するのかPDBファイルに保存するのか
    # /Z7 : デバッガーで使用する完全なシンボリック デバッグ情報も含んだオブジェクト ファイルを生成
    #    PDB ファイルはコンパイラによって生成されません。 
    # /Zi : デバッガーで使用するシンボリック デバッグ情報を含んだ個別の PDB ファイルを生成します。 デバッグ情報はオブジェクト ファイルや実行可能ファイルに含められない
    #       また、最適化には影響しません。
    # /ZI : エディット コンティニュ機能をサポートする形式で PDB ファイルを生成します。
    #        ほとんどの最適化処理はエディット コンティニュと互換性がないため、
    #       /ZI を使用すると、コード内の #pragma optimize ステートメントが無効になります。
    # /FD : /Zi,/ZIで作成されるPDBファイルのファイル名を指定

cl /Zi /FdPROG.PDB <ソースファイル>

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
