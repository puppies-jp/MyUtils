# C++についての簡易めも

[mapの使い方](#map)

## <a name=map>Map</a>

久々にC++を使ったらマップの使い方を忘れていたので、簡単にまとめておく、、

[基本的な使い方](#map_basic)
[挿入削除](#insert_delete)
[小ネタ](#additional)

[map cpprefjp](https://cpprefjp.github.io/reference/map/map.html)
[参考：std::mapまとめ](https://qiita.com/_EnumHack/items/f462042ec99a31881a81)

### <a name=map_basic>基本的な使い方</a>

```cpp
#include <map>
std::map<std::string,int> salary;

/// 普通にmapにインサートされる
salary["John"] = 1400;
salary["Tom"] = 1000;
salary["Harry"] = 0;

int a = salary["John"]; // 1400
int b = salary["Tom"]; // 1000
int c = salary["Harry"]; // 0


/// 🚨参照しただけで追加されるので注意がいる。
std::map<int,int> m{};
cout << m.size() << endl; // 何も要素を追加していないので当然 0
cout << m[1] << endl; // 要素は存在しない, 自動で0が追加される
cout << m.size() << endl; // 自動で要素が追加されているので 1

/// 🚨keyを指定して参照(atでkeyがない場合、例外が発生するので注意)
std::map<int,int> m{};
cout << m.size() << endl; // 何も要素を追加していないので当然 0
cout << m.at(1) << endl; // 要素は存在しない, Abort! out_of_range


/// 🌟[]で自動的に値を挿入させず、例外も出さずにキーの指定をする場合
/// 🌟ifの中身はcountでもいける。なければ0あれば1となるので。   dictionary.count("hoge")
std::map<std::string, unsigned> dictionary{
    {"John", 1000},{"Tom", 1400},{"Harry", 800}
};

// if ( dictionary.count("hoge") == 1 ) 
if ( auto iter = dictionary.find("hoge"); iter != end(dictionary) ) 
{std::cout << iter->second << std::endl;} 
else {std::cout << "not exists" << std::endl;}

/// 🌟マップのイテレーション(順番はless<Key>の順となる)
for (const auto& [key, value] : dictionary){
    std::cout << key << " => " << value << "\n";
}

```

---
---

### <a name=insert_delete> mapの挿入/削除</a>

```cpp
std::map<int,int> dic{};
dic.insert(std::make_pair(1,3));


std::map<int,int> dic{
    {1,2},
    {2,3},
    {3,4}
};

dic.erase(1); // (1,2)は消える
// イテレータからの削除もできる。
// dic.erase(begin(dic)); // 最初の要素(1,2)は消える

// (2,3)
// (3,4)
for (auto const& [k, v] : dic)
    printf("(%d,%d)\n", k, v); 
```

### <a name=additional>小ネタ</a>

#### 文字列比較のコストを下げる

- 第3引数で`std::less<>`を指定するだけ。

```cpp
  // 比較関数オブジェクトをデフォルトのstd::less<Key>の代わりにstd::less<> (std::less<void>と同じ) と指定する。
  // 検索関係のインタフェースに文字列リテラルを指定した際に
  // キーのためのstd::stringオブジェクトが作られないので余分なメモリ確保を回避できる
  std::map<std::string, int, std::less<>> m = {
    {"Alice", 3},
    {"Bob", 1},
    {"Carol", 4}
  };

  // 🌟以下となる
  // 文字列リテラル"Bob"からstd::stringオブジェクトが作られない
  m["Bob"] = 2;
  // 文字列リテラル"Carol"からstd::stringオブジェクトが作られない
  auto it = m.find("Carol");

```

#### mapの順番について

```cpp
namespace std {
  template <
        class Key,
        class T,
        class Compare = less<Key>,
        class Allocator = allocator<pair<const Key, T> >
  >
  class map;

  namespace pmr {
    template <class Key, class T, class Compare = less<Key>>
      using map = std::map<Key, T, Compare,
                           polymorphic_allocator<pair<const Key, T>>>;  // C++17から
  }
}
```

mapの内部では要素がキーでソートされている。
そのためmapはキーがソートできる`operator<`が定義されている型であることを要求される。
ソートの手段についてコンストラクタの`第3引数`で比較用の`less<Key>`テンプレートが展開される。

*`つまり、第3引数`*で`Key`クラスの比較クラスを作る。
or Keyクラスに`operator<`を追加にることで任意のクラス,構造体をKeyにすることができる。
以下参照。

- クラスに`operatior<`を追加する場合

```cpp
#include <iostream>
#include <map>
#include <string>
#include <tuple>

// 要素がひとつの場合
struct MyInt {
  int value;
};

　　// C++20から <=>で比較演算子が定義できるようになったらしい。(どう比較してるかは知らん、、)
 //friend auto operator<=>(const MyInt&, const MyInt&) = default;
 bool operator<(const MyInt& a, const MyInt& b) noexcept {
  return a.value < b.value;
}

// 要素が複数の場合
struct Person {
  int id;
  int age;
  std::string name;
};

  // C++20から <=>で比較演算子が定義できるようになったらしい。(どう比較してるかは知らん、、)
  //friend auto operator<=>(const Person&, const Person&) = default;
bool operator<(const Person& a, const Person& b) noexcept {
  // キーとして比較したい要素を列挙する(tieでid,age,nameの順で比較してくれるらしい)
  return std::tie(a.id, a.age, a.name) < std::tie(b.id, b.age, b.name);
}

int main() {
  std::map<MyInt, int> m1 {
    {MyInt{1}, 3},
    {MyInt{2}, 1},
    {MyInt{3}, 4},
  };
  std::cout << m1[MyInt{2}] << std::endl;

  std::map<Person, int> m2 {
    {Person{1, 18, "Alice"}, 3},
    {Person{2, 30, "Bob"}, 1},
    {Person{3, 30, "Carol"}, 4},
  };
  std::cout << m2[Person{2, 30, "Bob"}] << std::endl;
}
```

- 第三引数で比較クラスを作成する場合

```cpp
#include <iostream>
#include <map>
#include <string>
#include <tuple>

// 要素がひとつの場合
struct MyInt {
  int value;
};

struct MyIntLess {
  bool operator()(const MyInt& a, const MyInt& b) const noexcept {
    return a.value < b.value;
  }
};

// 要素が複数の場合
struct Person {
  int id;
  int age;
  std::string name;
};

struct PersonLess {
  bool operator()(const Person& a, const Person& b) const noexcept {
    // キーとして比較したい要素を列挙する
    return std::tie(a.id, a.age, a.name) < std::tie(b.id, b.age, b.name);
  }
};

int main() {
  std::map<MyInt, int, MyIntLess> m1 {
    {MyInt{1}, 3},
    {MyInt{2}, 1},
    {MyInt{3}, 4},
  };
  std::cout << m1[MyInt{2}] << std::endl;

  std::map<Person, int, PersonLess> m2 {
    {Person{1, 18, "Alice"}, 3},
    {Person{2, 30, "Bob"}, 1},
    {Person{3, 30, "Carol"}, 4},
  };
  std::cout << m2[Person{2, 30, "Bob"}] << std::endl;
}
```
