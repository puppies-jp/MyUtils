# プリプロセッサ関連 についてまとめる

---

## プリプロセッサ使い方メモ

### 使い方 その 1

```cpp
/* 使い方　その1------ */
// ここで var(i)という定義をする。
// var(1)とすると 下記のように展開される。
// printf("var1　= %d",var1)
#define var(i) printf("var" #i " = %d\n", var##i)

// 🌟引数の数が違う、同じ関数に置き換える
/* 可変長だとこんな感じ */
#define DEBUG_PRINT(...)  printf(__VA_ARGS__)
#define DEBUG_PRINT2(fmt, ...)  printf(fmt, __VA_ARGS__)


// 引数の数が違う、別の関数に置き換える
// 🌟 こんな感じで引数が2つだったらsum2,3つだったらsum3の関数に置き換えることができる。
#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define SUM(...) GET_MACRO(__VA_ARGS__, sum3, sum2)(__VA_ARGS__)


void check_preprocesser1()
{
    cout << "Start Check processer" << endl;
    int var1 = 11, var2 = 22;
    var(1);
    var(2);
    cout << "End Check processer" << endl;
}
```

- 出力結果

```shell
Start Check processer
var1 = 11
var2 = 22
End Check processer
```

### 使い方 その 2

```cpp
/* 使い方 その2------ */

// 関数の呼び方を変える。
void func1(int i)
{
    cout << "[" << __func__ << "]:"
         << "input: " << i << endl;
}

void func2(int i)
{
    cout << "[" << __func__ << "]:"
         << "input: " << i << endl;
}

void check_preprocesser2()
{
    cout << "[Check]Start Preprocesser" << endl;
#define FUNC(xi, y) func##xi(y)
    FUNC(1, 10);
    FUNC(2, 20);
#undef FUNC
    cout << "[Check]end Preprocesser" << endl;
}
```

- 出力結果

```shell
[Check]Start Preprocesser
[func1]:input: 10
[func2]:input: 20
[Check]end Preprocesser
```

### 使い方 その 3 条件付きコンパイル

```cpp
#ifdef マクロ名  // #ifndefとすることもできる
    処理２
#else
    処理３
#endif
```

- 例

```cpp
// build時にTESTがoptionでついていると、
// ifdefを通り、この部分を読み込まれてhogeが"base2"へ置き換わる。
#ifdef TEST
#include "sub/base2.cpp"
#define hoge base2
#endif

~~ 略 ~~

// #undef hogeとすることで
// ここより後に定義を持ち出さないようにすることができる。
#undef hoge
```

- `cmake`だとこんなん感じ -DTEST の on/off をつける。
  (付けない場合、デフォルトで else に移る)

```shell
cmake ../src -DTEST=on
```

```cmake
if(TEST)
    message("|[preprocessor]test option build   ")
    # ここでTESTを定義する。
    add_definitions(-DTEST)
else()
    message("|[preprocessor]no   option build"  )
endif()
```

---

## プリプロセッサ展開結果を出力する

- gcc 環境でプリプロセッサ展開結果を確認する

  - gcc であれば下記のようにオプション -E を付加してやれば、プリプロセッサ処理直後のデータが表示できる。

  ```shell
   gcc -E main.c
  ```

  - gcc に`-save-temps オプション` をつけてビルドすると以下のように出力できる。

    1. プリプロセッサ出力: foo.i (C の場合) / foo.ii (C++ の場合)
    2. アセンブリコード: foo.s

- cmake ならこんな感じ

```cmake
ifneq ($(SAVETEMPS),)
CFLAGS += -save-temps=obj
endif
```

```shell
make SAVETEMPS=1
```

- Windows 環境でプリプロセッサ展開結果を確認する

  1. ソリューションエクスプローラ上で展開後の結果を見たい cpp ファイルを右クリック ⇒ プロパティ ⇒ プリプロセッサ ⇒ 前処理済みファイルの生成 ⇒ 行番号つきか行番号なしを選択
  2. 再びソリューションエクスプローラ上で展開後の結果を見たい cpp ファイルを右クリック ⇒ コンパイル
  3. cpp が置かれているのと同じファイル階層に、cpp と同じ名前だけど拡張子が.i になってるファイルができてる
