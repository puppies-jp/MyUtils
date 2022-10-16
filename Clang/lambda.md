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
