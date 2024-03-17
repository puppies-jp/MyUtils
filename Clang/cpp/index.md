# C++の基本的なやつを書いていく

- [mapについて](map)
- [spanクラスについて](#span)
  spanクラスは`連続したオブジェクトの部分シーケンスを参照するのに使われる`

---
---

<a name=span>spanクラスについて</a>

```cpp
#include <iostream>
#include <span>
#include <vector>
#include <utility>

// メモリ連続性をもつあらゆる範囲を出力する関数。
// std::spanオブジェクトはコピーで受け取るのが基本的な使い方
template <class T, std::size_t Extent>
void print(std::span<T, Extent> s)
{
  const char* delimiter = "";

  std::cout << '{';
  for (const T& x : s) {
    std::cout << std::exchange(delimiter, ",") << x;
  }
  std::cout << '}' << std::endl;
}

int main()
{
  std::vector<int> v = {1, 2, 3, 4, 5};
  int ar[] = {1, 2, 3, 4, 5};

  // spanに変換してコンテナ全体を出力
  print(std::span{v});
  // {1,2,3,4,5}

  // コンテナの一部の要素を出力
  print(std::span{v}.subspan(1, 3));
  // {2,3,4}

  // ポインタと要素数を指定した範囲を参照して、
  // 範囲for文を使用する
  print(std::span<int>{ar, 3});
  // {1,2,3}

}
```
