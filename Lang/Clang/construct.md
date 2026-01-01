# cpp におけるコンストラクト/デストラクトについてまとめる

1. [コピーコンストラクタ/コピー代入演算子について](#copy)

---

## <a name=copy>コピーコンストラクタ/コピー代入演算子について</a>

コピーコンストラクタ: `オブジェクトを「同じ型の別のオブジェクト」で初期化するときに使われる。`

コピー代入演算子: `オブジェクトを「同じ型の別のオブジェクト」で代入するときに使われる。`

メンバ変数間で何を共有するか、どう使うかについては検討が必要

```cpp
#include <iostream>

class Widget {
 public:
  explicit Widget(int a) { std::cout << "コンストラクタ" << std::endl; };
  Widget(const Widget& rhs) { std::cout << "コピーコンストラクタ" << std::endl; };
  Widget& operator=(const Widget& rhs) { 
    std::cout << "コピー代入演算子" << std::endl; 
    return *this;
  };
  ~Widget(){};
};

int main(int argc, char* argv[]) {
  std::cout << argv[0] << std::endl;
  Widget a(1);   // コンストラクタ
  std::cout << "代入前(代入前後でポインタは変わらない) "<< std::endl;
  std::cout << "aのアドレス : " << &a << std::endl;
  Widget b(a);   // コピーコンストラクタ
  a        = b;  // コピー代入演算子
  Widget c = b;  // コピーコンストラクタ

  std::cout << "aのアドレス : " << &a << std::endl;
  std::cout << "bのアドレス : " << &b << std::endl;
  std::cout << "コピーコンストラクタ(bと同じポインタになるわけではない) "<< std::endl;
  std::cout << "cのアドレス : " << &c << std::endl;
}
```

```bash
(base)root@ubuntu:~/workspace/MyUtils/Clang/src/etc# ./copyConstruct.out 
./copyConstruct.out
コンストラクタ
代入前(代入前後でポインタは変わらない) 
aのアドレス : 0x7ffdf60d72d5
コピーコンストラクタ
コピー代入演算子
コピーコンストラクタ
aのアドレス : 0x7ffdf60d72d5
bのアドレス : 0x7ffdf60d72d6
コピーコンストラクタ(bと同じポインタになるわけではない) 
cのアドレス : 0x7ffdf60d72d7
```