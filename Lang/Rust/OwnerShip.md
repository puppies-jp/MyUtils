# Rustの所有権と参照/借用

Rustではメモリをコンパイル段階でメモリ安全を担保するために所有権という考え方がある。

## 所有権規則

前提として、以下の3つの規則がある。

1. Rustの各値は、所有者と呼ばれる変数と対応している。
2. `いかなる時も所有者は一つ`である。  
   (所有者が変われば、元所有者は触れない。)
3. 所有者がスコープから外れたら、値は破棄される。
   `(所有者を一つにすることで、二重解放を避ける)`

## データ操作まとめ

- [move](#move)
- [clone/copy](#clone)
- [関数による所有権移動](#function)
- [構造体の所有権](#struct)

## <a name="move">データと変数の相互作用(move)</a>

```rust
// 🌟数値型は固定で、普通にスタックに積まれて使えるらしい
let x = 5;
let y = x;

// 🌟s2にs1の持つメモリの所有権が映るので、
// s1を使おうとするとエラーを起こす
let s1 = String::from("hello");
let s2 = s1;

// println!("{}, world!", s1); 
println!("{}, world!", s2);
```

## <a name="clone">データと変数の相互作用(clone/copy)</a>

```rust
// 🌟clone ヒープをまるッとコピーする
// しかし、負荷が高いっぽいので使用は避けたいところ。。。
let s1 = String::from("hello");
let s2 = s1.clone();

println!("s1 = {}, s2 = {}", s1, s2);

// 🌟copy スタックに積まれる。
let x = 5;
let y = x;

println!("x = {}, y = {}", x, y);
```

RustにはCopyトレイトと呼ばれる特別な注釈があり、 整数のようなスタックに保持される型に対して配置することができます。
型がCopyトレイトに適合していれば、代入後も古い変数が使用可能になります。

以下が、スタックに積まれる形となる

- あらゆる整数型。u32など。
- 論理値型であるbool。trueとfalseという値がある。
- あらゆる浮動小数点型、f64など。
- 文字型であるchar。
- タプル。ただ、Copyの型だけを含む場合。
  例えば、(i32, i32)はCopyだが、
  `(i32, String)`は違う。

## <a name="function">関数の引数による所有権</a>

```rust
fn main() {
    let s = String::from("hello");  // sがスコープに入る
    takes_ownership(s);             // sの値が関数にムーブされ...
                                    // ... ここではもう有効ではない

    let x = 5;                      // xがスコープに入る
    makes_copy(x);                  // xも関数にムーブされるが、
                                    // i32はCopyなので、この後にxを使っても
                                    // 大丈夫

} // ここでxがスコープを抜け、sもスコープを抜ける。ただし、sの値はムーブされているので、何も特別なことは起こらない。
  //

fn takes_ownership(some_string: String) { // some_stringがスコープに入る。
    println!("{}", some_string);
} // ここでsome_stringがスコープを抜け、`drop`が呼ばれる。後ろ盾してたメモリが解放される。
  // 

fn makes_copy(some_integer: i32) { // some_integerがスコープに入る
    println!("{}", some_integer);
} // ここでsome_integerがスコープを抜ける。何も特別なことはない。
```

```rust
fn main() {
    // gives_ownershipは、戻り値をs1にムーブする
    let s1 = gives_ownership();
    
    // s2がスコープに入る
    let s2 = String::from("hello");     

    // 🌟ここでs2はmoveされて有効じゃなくなる
    // s2はtakes_and_gives_backにムーブされ戻り値もs3にムーブされる
    let s3 = takes_and_gives_back(s2);  

} // ここで、s3はスコープを抜け、ドロップされる。s2もスコープを抜けるが、ムーブされているので、
  // 何も起きない。s1もスコープを抜け、ドロップされる。

fn gives_ownership() -> String {
    // gives_ownershipは、戻り値を呼び出した関数にムーブする
    // some_stringがスコープに入る
    let some_string = String::from("hello"); 
    
    // some_stringが返され、呼び出し元関数にムーブされる
    some_string
}

// takes_and_gives_backは、Stringを一つ受け取り、返す。
fn takes_and_gives_back(a_string: String) -> String { // a_stringがスコープに入る。

    a_string  // a_stringが返され、呼び出し元関数にムーブされる
}
```

- 🌟毎回所有権を移動し、引数の所有権移動されるのではうざいが、
  `tuple`で返り値にすることで回避する手法もあるが、
  参照で回避できる。
  参照については *[#reference](./grammar.md#reference)* で語る。

```rust
fn main() {
    let s1 = String::from("hello");

    let (s2, len) = calculate_length(s1);

    //'{}'の長さは、{}です
    println!("The length of '{}' is {}.", s2, len);
}

fn calculate_length(s: String) -> (String, usize) {
    let length = s.len(); // len()メソッドは、Stringの長さを返します
    return (s, length)
}
```

## <a name=struct>構造体の所有権</a>

基本的に何も指定しなければ`move`となる。

- `derive`アノテーションをつけることで、`copy`,`clone`の実装をコンパイラでやってくれるらしい、`debug`をつけるとfmtで表示するときに自動でメンバ変数の表示とかを作ってくれる。

```rust
// 🌟deriveでコンパイラが、Debug,Clone,Copyが実装する。
#[derive(Debug, Clone, Copy)]
struct Point { x: i32, y: i32 } // 全フィールドが Copy なら derive できる

// i32 は Copy だが、Point は自動で Copy にならない
#[derive(Debug)]
struct Point2 { x: i32, y: i32 } 

fn main() {    
    let p1 = Point { x: 1, y: 2 };
    let p2 = p1; // コピーされる（ムーブではない）
    println!("{:?}", p1); // 問題なく使える
    println!("{:?}", p2);

    //🌟 Tip:こういう書き方をすると、取り出す各フィールドが Copy ならコピーされ、Copy でなければムーブされます。
    let Point { x, y } = p;

    //deriveでcopyがない場合
    let p1 = Point2 { x: 1, y: 2 };
    let p2 = p1; // p1 はムーブされる（Point2 が Copy でないため）
    // println!("{:?}", p1); // コンパイルエラー: use of moved value: `p1`
}
```
