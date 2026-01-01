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
