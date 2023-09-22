# lambda式について

- [変数/キャプチャについて](#args)
- [関数ポインタへの変換](#function)

---

## 基本系

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

## <a name=args>引数について</a>

- [キャプチャ記法](https://cpprefjp.github.io/lang/cpp11/lambda_expressions.html#capture)
  - ラムダ式の外にある自動変数を、ラムダ式内で参照できるようにする「キャプチャ(capture)」という機能がある。キャプチャは、ラムダ導入子(lambda-introducer)と呼ばれる、ラムダ式の先頭にある[ ]ブロックのなかで指定する。
  - コピーキャプチャ :`環境にある変数をコピーして、ラムダ式のなかで使用する`
    - `[=]`:`デフォルトで環境にある変数をコピーして、ラムダ式のなかで使用する`
    - `[x]`:`変数xをコピーして、ラムダ式のなかで使用する`
    - `[&,x]`:`デフォルトで参照キャプチャ、変数xのみコピーして、ラムダ式のなかで使用する`
  - 参照キャプチャ :`環境にある変数を参照して、ラムダ式のなかで使用する`
    - `[&]`:`デフォルトで環境にある変数を参照して、ラムダ式のなかで使用する`
    - `[&x]`:`変数xを参照して、ラムダ式のなかで使用する`

🚨 `ラムダ式がひとつ以上の変数を参照キャプチャしている場合、参照している変数の寿命が切れたあとの、ラムダ式のコピーと呼び出しの動作は未定義。`

```cpp
#include <iostream>
using namespace std;

int main() {
  int i = 0;
  // []にはキャプチャリストと言ってiを固定値にしている。
  auto f = [i](int j) {
    cout << "i = " << i << endl;
    cout << "j = " << j << endl;

    // 🌟戻り値の型は推論で判定してもらう,,,
    // returnがなきゃvoidになるっぽい
    return i+j;
  };

  // 🌟 []() ->type{} で戻り値の型を指定する
  auto f2 = [i](int j) -> int {
    cout << "i = " << i << endl;
    cout << "j = " << j << endl;

    return i+j;
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

---

## <a name=function>関数ポインタへの変換</a>

```cpp

void foo(int(*fp)(int, int))
{
  int result = fp(2, 3);
  std::cout << result << std::endl;
}

int main()
{
  // lambda式を関数ポインタとして扱う
  auto lmbd = [](int a, int b) { return a + b; }
  foo(lmbd);
}
```
