# C/C++でSO/DLLライブラリを作成してC#から実行する

なんだかんだライブラリとしては`C`の形式じゃないといけないっぽい

- C++側ライブラリ(@Mac)

ビルドコマンド

```sh
g++ -fPIC -shared test.cpp -o CxxLibrary.so -rdynamic -ldl -std=c++2a
```

```cpp
#include <stdio.h>

extern "C" {
    // Windows では　__declspec(dllexport)　をつけて定義すること
    int add(int a, int b);
    int* arr_a(int length);
    void arr_b(int* arr, int length);
}

int add(int a, int b) {
    return a + b;
}

int* arr_a(int length) {
    int* arr = new int[length];

    for (int i = 0; i < length; i++) {
        arr[i] = i;
    }
    return arr;
}

void arr_b(int* arr, int length) {
    for (int i = 0; i < length; i++) {
        arr[i] = i;
    }
}
```

- C#側アプリケーション(@Mac)

```bash
mcs ファイル名.cs -lib:CxxLibrary.so 
mono exeファイル
```

```cs
using System.Runtime.InteropServices;

[DllImport("CxxLibrary.dll")]
private static extern int add(int a, int b);

[DllImport("CxxLibrary.dll")]
private extern static IntPtr arr_a(int length);

[DllImport("CxxLibrary.dll")]
private extern static void arr_b(IntPtr arr, int length);

void main(){

    // add 使い方
    int addResult = add(1, 2);
    
    // arr_a 使い方---------------------
    const int NUM = 5;
    int[] arr = new int[NUM];
    // 戻り値のポインタをIntPtrで受け取る
    IntPtr ptr = arr_a(NUM);
    // マネージ配列へコピー
    Marshal.Copy(ptr, arr, 0, NUM);
    //--------------------------------

    // arr_b 使い方 -------------------
    const int NUM = 5;
    int[] arr = new int[NUM];
    // アンマネージ配列のメモリを確保
    IntPtr ptr = Marshal.AllocCoTaskMem(Marshal.SizeOf(typeof(int)) * NUM);

    // 引数でポインタを渡す
    arr_b(ptr, NUM);
    // マネージ配列へコピー
    Marshal.Copy(ptr, arr, 0, NUM);
    // アンマネージ配列のメモリを解放
    Marshal.FreeCoTaskMem(ptr);
    // -------------------------------

}
```
