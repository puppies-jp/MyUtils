use mysql::*;
use mysql::prelude::*;
use std::sync::LazyLock;

static POOL: LazyLock<mysql::Pool> = LazyLock::new(|| {
    let url = "mysql://admin:password@localhost:53306/mydatabase";
    return Pool::new(url).unwrap()
});

#[derive(Debug,serde::Serialize,serde::Deserialize)]
pub struct User {
    pub id : Option<i32>,
    pub name: String,
    pub age: i32,
}

pub fn create_conn() -> Result<PooledConn,u32>
{
    let conn = POOL.get_conn().expect("Error connect mysql pool");
    return  Ok(conn);
}

pub fn create_table(conn: &mut PooledConn) -> Result<()> {
    conn.query_drop(
        r"
        CREATE TABLE IF NOT EXISTS users(
        id  INT NOT NULL  primary key auto_increment,
        name VARCHAR(255) NOT NULL,
        age  INT          NOT NULL)
        "
    )?;
    Ok(())
}

pub fn insert_users(conn: &mut PooledConn, users: &[User]) -> Result<()> {
    conn.exec_batch(
        r"INSERT INTO users (name, age) VALUES
        (:name, :age)",
        users.iter().map(|p| params! {
            "name" => p.name.to_string(),
            "age" => p.age,
        })
    )?;
    Ok(())
}

pub fn select_users(conn: &mut PooledConn) -> Result<Vec<User>> {
    let users: Vec<User> = conn.query_map(
        "SELECT id, name, age FROM users",
        |(id , name, age)| User { id, name, age },
    )?;
    Ok(users)
}
