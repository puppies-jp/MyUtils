# C/C++ の引数についてまとめる

## 可変長引数

- 可変長引数に関しては以下の 2 つの方法が挙げられる。

  - 古くからある方法

    ```cpp
    int printf(const char *fmt, ...)
    {

        // 🌟 ここのfmtはあくまでfmt以降の引数をまとめるためのもので、
        // 以降の処理でfmtとapを一緒に使う必要はない。
        va_list ap; /* 引数のリストを生成 */
        va_start(ap, fmt);
        int ret = vprintf(fmt, ap);

        // 変数の型　　=　va_arg(args, 変数の型)
        // ってやることで、引数を順番に型をキャスとして抜き出してくる。
        va_end(ap); /* va_listの初期化 */
        return ret;
    };
    ```

  - template を使った方法

    ```cpp
    #include <iostream>
    #include <utility>

    // パラメータパックが空になったら終了
    void print() {}

    // ひとつ以上のパラメータを受け取るようにし、
    // 可変引数を先頭とそれ以外に分割する
    template <class Head, class... Tail>
    void print(Head&& head, Tail&&... tail)
    {
    // 先頭の引数を出力する。
    std::cout << head << std::endl;
    // パラメータパックtailをさらにheadとtailに分割、再起呼び出し
    print(std::forward<Tail>(tail)...);
    }

    int main()
    {
    print(1, 'a', "hello");
    }
    ```

---

- 以下
  - 値渡し = 変数の値を別の場所にコピーする。アドレスは異なる。
  - 参照渡し = 同じ値、同じアドレスをもつ変数を作り出す。変数名を変えただけ。
  - アドレス渡し = 変数のアドレスを値としてコピーする。アドレスは異なる。

---

## 参照渡し

- 仮にこう書いたとして、結果は`0`となる。

```cpp
int global_int=0;
int &f(){
    return global_int;
}

int main(){
    int x;
    x = f();
    x++;
    cout << global_int << "\n";
}
```

```cpp
0
```

- `1` でなく、`0` となったのは以下に書いたのと同じ動作のためである。

```cpp
int global_int=0;

int main(){
    int x;
    x = global_int;
    x++;
    cout << global_int << "\n";
}
```

- こう書くことでアドレスと値を返すことができる。

```cpp
#include <iostream>
static int global_int=0;
int &f()
{
    return global_int;
}

int main()
{
    int& x = f();
    x++;
    std::cout << global_int << std::endl;
}
```

## ダイレクトパラメータアクセス

- printfに`$`を使ってやることで、引数の何番目の数字を使用するのかを直で指定できる

- 使い方はこんな感じ

```c
printf("%2$d",1,2,3);
```

- 出力例
  - ⚠️他の`%d`などのアクセス順に影響してない

```sh
(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# cat dyrectPrm.c 
#include<stdio.h>

int main()
{
    printf("%d %d %1$d %d %d %d \narg2: %2$d\n"
		,1,2,3,4,5);
	return 0;
}

(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# ./a.out
1 2 1 3 4 5 
arg2: 2
(base) root@9ea233d8d241:~/Desktop/LeakDetect/OverFlow# 
```
