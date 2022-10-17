# lambda式について

```cpp
#include <iostream>
using namespace std;

int main() {
    // あくまで定義だけ(x,x)で引数を定義できる。
    // -> type で戻り値を
    auto f = [](int i, int j) -> int {
    return i * j;
  };

  cout << f(2, 3) << endl;
}

```

---

## 変数について

- [キャプチャ記法](https://cpprefjp.github.io/lang/cpp11/lambda_expressions.html#capture)
  - ラムダ式の外にある自動変数を、ラムダ式内で参照できるようにする「キャプチャ(capture)」という機能がある。キャプチャは、ラムダ導入子(lambda-introducer)と呼ばれる、ラムダ式の先頭にある[ ]ブロックのなかで指定する。
  - コピーキャプチャ :`環境にある変数をコピーして、ラムダ式のなかで使用する`
    - `[=]`:`デフォルトで環境にある変数をコピーして、ラムダ式のなかで使用する`
    - `[x]`:`変数xをコピーして、ラムダ式のなかで使用する`
    - `[&,x]`:`デフォルトで参照キャプチャ、変数xのみコピーして、ラムダ式のなかで使用する`
  - 参照キャプチャ :`環境にある変数を参照して、ラムダ式のなかで使用する`
    - `[&]`:`デフォルトで環境にある変数を参照して、ラムダ式のなかで使用する`
    - `[&x]`:`変数xを参照して、ラムダ式のなかで使用する`

```cpp
#include <iostream>
using namespace std;

int main() {
  int i = 0;
  // []にはキャプチャリストと言ってiを固定値にしている。
  auto f = [i](int j) {
    cout << "i = " << i << endl;
    cout << "j = " << j << endl;
  };

  i = 1;
  f(i);
}

```

出力

```sh
g++ -std=c++11 test.cpp
./a.out 
i = 0
j = 1
```
