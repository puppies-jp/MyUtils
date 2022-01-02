# C/C++ の引数についてまとめる

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
