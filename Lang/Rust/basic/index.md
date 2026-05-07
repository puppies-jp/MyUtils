# Grammar and basic usage of Rust

## `Result`,`Option`の使い方

- `Result`は、成功した場合は`Ok(T)`、失敗した場合は`Err(E)`を返す列挙型である。
- `Option`は、値が存在する場合は`Some(T)`、存在しない場合は`None`を返す列挙型である。

以降の章でエラーハンドリングについてまとめるが、

`match式`や`if let`を使用して、`Result`や`Option`の値を処理することが多い。
また、`except`や`unwrap`を使用することで、エラー処理を行う。

```rust

println!("Result and Option------------------------");
println!("Result-----------------------------------");
let result: Result<i32, String> ;//= Ok(40);
result = Err("Error occurred".to_string());
match result {
    Ok(num) => println!("Parsed number: {}", num),
    Err(ref e) => println!("Failed to parse: {}", e),
}
println!("Option-----------------------------------");
let option: Option<i32>;
option = Some(10);
match option {
Some(num) => println!("Parsed number: {}", num),
None => println!("Failed to parse"),
}
```

[sample ソース](./src/ErroHandle/ErrorHandle.rs)

## エラーハンドリング

Rustのエラーハンドリングは、`Result型`で行われる。
`Result型`は、成功した場合は`Ok(T)`、失敗した場合は`Err(E)`を返す列挙型である。
このため、エラー処理は、`match`式や`unwrap`メソッドなどを使用して行われる。
また、`?`演算子を使用することで、エラーを簡単に伝播させることもできる。

`unwarp`は、失敗したことで以後の処理ができない場合に使用することが多い。
例えば、`設定ファイル`が読み込めなかった場合などは、`unwarp`,`except`を使用してエラーを処理することが多い。

- `match` or `if let`を使用したエラー処理の例

[sample ソース](./src/ErroHandle/ErrorHandle.rs)

```rust
fn error_example() {
    // OKとなるパターン
    let num_str = "10";
    //let num_str = "10fa";
    let result: Result<i32, std::num::ParseIntError> = num_str.parse();


    // match式を使用してエラー処理
    // Ok,Err両方のパターンを記述すること
    match result {
        Ok(num) => println!("Parsed number: {}", num),
        Err(ref e) => println!("Failed to parse: {}", e),
    }

    // Ok or Errorの場合のみ処理を分岐する場合
    if let Ok(num) = result {
        println!("Parsed number: {}", num);
    } else  if let Err(ref e) = result {
        println!("Failed to parse: {}", e);
    }    

    // 直接値を取り出す場合
    let result = num_str.parse()?;
    println!("Parsed number: {}", result);

}
```

- `unwrap`, `expect`を使用したエラー処理の例

```rust
fn error_example() {
    let num_str = "10";
    //let num_str = "10fa";
    let result: Result<i32, std::num::ParseIntError> = num_str.parse();

    // unwrapを使用してエラー処理
    // 成功した場合は値を返し、失敗した場合はパニックする
    //let num = result.unwrap();
    
    // expectを使用してエラー処理
    // 成功した場合は値を返し、
    // 失敗した場合は"指定したメッセージ"でパニックする
    let num = result.expect("Failed to parse number");
    println!("Parsed number: {}", num);
}

fn error_example2() {
    let opt: Option<i32> = None

    // unwrapを使用してエラー処理
    // Noneでない場合には値を返し、失敗した場合はパニックする
    //let num = opt.unwrap();
    
    // expectを使用してエラー処理
    // Noneでない場合には値を返し、
    // 失敗した場合は"指定したメッセージ"でパニックする
    let num = opt.expect("Failed to parse number");
    println!("Parsed number: {}", num);
}

```

## File Reading and Writing

- File読み込みについてまとめる(そのうち)
`std:fs` と `std::io`を使用して、ファイルの読み書きを行うことができる。
違いは、
- `std::fs`はファイルシステムに関する操作を提供し、
- `std::io`は入出力に関する操作を提供すること。

例えば、`std::fs::read_to_string`を使用して、ファイルの内容を文字列として読み込むことができる。
- また、`std::io::BufReader`を使用して、ファイルを行単位で読み込むこともできる。
fsは、ファイルの作成や削除などの操作も提供する。
- ioは、ファイルの読み書きに関する操作を提供するだけでなく、標準入力や標準出力などの入出力に関する操作やネットワークソケットの読み書きなども提供する。

[sample ソース](./src/FileAccess/FileRead.rs)