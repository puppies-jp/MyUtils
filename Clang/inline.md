# operator/inlineについて

`operator`/`inline`関係ないけど、単体だけだとそこまで書くことないのでまとめる。

## <a name=operator>operatorについて</a>

オペレータ関数の作り方はこんな感じ

```cpp
class TestClass
{
    int num;

public:
    //🌟+記号をオーバーロード(🌟引数は+の左辺値)
    TestClass operator+(TestClass r)
    {
        TestClass tc;
        tc.num = this->num + r.num;
        return tc;
    }
    //🌟-記号をオーバーロード(🌟引数は+の左辺値)
    TestClass operator-(TestClass r)
    {
        TestClass tc;
        tc.num = this->num - r.num;
        return tc;
    }
};
```

## <a name=inline>inline関数について</a>

```cpp
#include <iostream>

inline void logger(const char *file, int line, std::string src)
{
    std::cout << file << "[" << line << "]"
              << src << std::endl;
    std::cout << __PRETTY_FUNCTION__ << "["
              << __LINE__ << "]" << std::endl;
    return;
}

#define plogger(src) logger(__PRETTY_FUNCTION__, __LINE__, src)

int main()
{
    // 🌟プリプロセッサで行数を取れる。
    //   しかし、inline展開したとはいえ関数内の行数で出力される。
    plogger("What`s"); 

    plogger("What`s2");

    return 0;
}
```

- 出力内容

```sh
user@macbook Descripter % ./a.out             
int main()[16]What`s
void logger(const char *, int, std::string)[8]
int main()[18]What`s2
void logger(const char *, int, std::string)[8]
```
