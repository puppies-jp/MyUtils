# RustでMySQLを扱う

## MySQLクレート

RustでMySQLを扱うためのクレートとして、`mysql`クレートが一般的に使用されます。このクレートは、MySQLデータベースとの接続、クエリの実行、および結果の処理を簡単に行うための機能を提供しています。

### 依存関係の追加

Cargo.tomlファイルに以下の依存関係を追加します。

```toml
[dependencies]
mysql = "22.0.0" # 最新バージョンを確認して使用
```

### MySQLへの接続

MySQLデータベースに接続するための基本的なコードは以下のようになります。

```rust
use mysql::*;
use mysql::prelude::*;

fn main() -> Result<()> {
    // データベースへの接続情報を指定
    let url = "mysql://username:password@localhost:3306/database_name";
    
    // データベースに接続
    let pool = Pool::new(url)?;
    let mut conn = pool.get_conn()?;

    // クエリの実行例
    let result: Vec<(String, i32)> = conn.query("SELECT name, age FROM users")?;

    // 結果の処理
    for (name, age) in result {
        println!("Name: {}, Age: {}", name, age);
    }

    Ok(())
}
```


## 構造体と紐付けてクエリの結果を構造体にマッピングすることもできます。

```rust
use mysql::*;
use mysql::prelude::*;

#[derive(Debug, PartialEq, Eq)]
struct User {
    name: String,
    age: i32,
}

fn main() -> Result<()> {
    let url = "mysql://username:password@localhost:3306/database_name";
    let pool = Pool::new(url)?;
    let mut conn = pool.get_conn()?;

    // connectオブジェクトを使用して、テーブルを作成する
    // 参照渡しで渡すことで、関数内で変更した内容が呼び出し元にも反映されるようにする
    create_table(&mut conn)?;

    // データの挿入
    // 1.insertする構造体作成
    let users = vec![
        User { name: "Alice".to_string(), age: 30 },
        User { name: "Bob".to_string(), age: 25 },
    ];

    // 2.insertする関数に構造体の配列を渡す
    // 参照渡しで渡すことで、関数内で変更した内容が呼び出し元にも反映されるようにする
    insert_users(&mut conn, &users)?;

    // クエリの実行と構造体へのマッピング
    let users = select_users(&mut conn)?;

    for user in users {
        println!("{:?}", user);
    }

    Ok(())
}


fn create_table(conn: &mut PooledConn) -> Result<()> {
    conn.query_drop(
        r"CREATE TABLE IF NOT EXISTS users (
            name VARCHAR(255) NOT NULL,
            age INT NOT NULL
        )"
    )?;
    Ok(())
}

fn insert_users(conn: &mut PooledConn, users: &[User]) -> Result<()> {
    conn.exec_batch(
        r"INSERT INTO users (name, age) VALUES
        (:name, :age)",          
          users.iter().map(|p| params! {
            "name" => p.name,
            "age" => p.age,
        })
    )?;
    Ok(())
}

fn select_users(conn: &mut PooledConn) -> Result<Vec<User>> {
    let users: Vec<User> = conn.query_map(
        "SELECT name, age FROM users",
        |(name, age)| User { name, age },
    )?;
    Ok(users)
}

```
