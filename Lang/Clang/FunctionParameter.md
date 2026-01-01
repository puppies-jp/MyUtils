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

## 引数の挙動確認

> `結論`
> 参照渡しを引数に渡すと値渡しと違い、渡されたオブジェクト本体のポインタ、値で渡される(つまり同じオブジェクトを見にいく)

- 動作確認用ソース

```cpp
#include <stdio.h>
#include <iostream>

class test
{
public:
    int x = 0;
};

// 値渡し
void func_norm(test tmp)
{
    printf("%-10s: %p : %d\n", __func__, &tmp, tmp.x);
}

// 　参照渡し
void func_ref(test &tmp)
{
    printf("%-10s: %p : %d\n", __func__, &tmp, tmp.x);
}

// ポインタ渡し
void func_ptr(test *tmp)
{
    printf("%-10s: %p : %d\n", __func__, tmp, tmp->x);
}

int main()
{
    test tmp;
    tmp.x = 3;
    printf("%-10s: %p : %d\n", __func__, &tmp, tmp.x);
    func_norm(tmp);
    func_ref(tmp);
    func_ptr(&tmp);
    return 0;
}
```

- 出力結果

```shell
(base) root@9ea233d8d241:~/Desktop/LeakDetect/Descripter# g++ classtest.cpp -o cls.out
(base) root@9ea233d8d241:~/Desktop/LeakDetect/Descripter# 
(base) root@9ea233d8d241:~/Desktop/LeakDetect/Descripter# ./cls.out 
main      : 0x7ffc25c9b734 : 3
func_norm : 0x7ffc25c9b71c : 3
func_ref  : 0x7ffc25c9b734 : 3
func_ptr  : 0x7ffc25c9b734 : 3
(base) root@9ea233d8d241:~/Desktop/LeakDetect/Descripter# 

```