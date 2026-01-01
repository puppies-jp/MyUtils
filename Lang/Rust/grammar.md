# Rustの文法

Rustの構文についてまとめる

---

## 所有権(ownership)/束縛(bind)

Rustのキーワードとして`mutable(可変)`,`immutable(不変)`を覚える必要がある。
`immutable`な変数に代入する時点でコンパイルエラーを起こす。
Rustではどうしても書き換えたい場合の書き換え方も存在するらしい  (unsafeという形だが、、)

```rust
let x = 5; // immutable
const y = 6; //immutable

let mut z = 7; //mutable
```

---

## 型

|型|表記|
|--|--|
|signed integers|i8,i16,i32,i64,i128,isize(pointer size)|
|unsigned integers|u8,u16,u32,u64,u128,usize(pointer size)|
|floating point|f32,f64|
|char|'a','b'|
|bool|'true' or 'false'|
|Array|[1,2,3]|
|Tupple|(1,true)|

```rust
fn main() {
    // Variables can be type annotated.
    let logical: bool = true;

    let a_float: f64 = 1.0;  // Regular annotation
    let an_integer   = 5i32; // Suffix annotation

    // Or a default will be used.
    let default_float   = 3.0; // `f64`
    let default_integer = 7;   // `i32`

    // A type can also be inferred from context.
    let mut inferred_type = 12; // Type i64 is inferred from another line.
    inferred_type = 4294967296i64;

    // A mutable variable's value can be changed.
    let mut mutable = 12; // Mutable `i32`
    mutable = 21;

    // Error! The type of a variable can't be changed.
    mutable = true;

    // Variables can be overwritten with shadowing.
    let mutable = true;

    /* Compound types - Array and Tuple */

    // Array signature consists of Type T and length as [T; length].
    let my_array: [i32; 5] = [1, 2, 3, 4, 5];

    // Tuple is a collection of values of different types
    // and is constructed using parentheses ().
    let my_tuple = (5u32, 1u8, true, -5.04f32);
    // タプル要素アクセス
    println!("tuple first value: {}", my_tuple.0);
    println!("tuple second value: {}", my_tuple.1);
}
```

### 配列スライス

```rust
let mut array: [i32; 4] = [0; 4];
array[1] = 1;
array[2] = 2;
        
assert_eq!([1, 2], &array[1..3]);//1番目から3番目まで
assert_eq!([1, 2,0], &array[1..4]);//1から4番目まで
assert_eq!([1, 2,0], &array[1..]);//1から最後まで

// This loop prints: 0 1 2
for x in array {
    print!("{x} ");
}
    
for x in &array[1..2] 
{ 
 print!("{} ",x)
}
```

## 構造体

```rust
struct User {
    username: String,
    email: String,
    sign_in_count: u64,
    active: bool,
}

let email = String::from("someone@example.com"); 
let mut user1 = User {    
    //代入する変数名とメンバ変数名が同じならそのまま渡せるらしい
    //可読性とかでやらんほうがいい気もするが、、
    email,
    username: String::from("someusername123"),
    active: true,
    sign_in_count: 1,
};

user1.email = String::from("anotheremail@example.com");
```
