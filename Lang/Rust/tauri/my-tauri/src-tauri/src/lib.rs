use crate::query::{User};

// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

mod query;

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![greet,login,my_custom_command,create_table,select_table,insert_table]
        )
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}

#[tauri::command]
fn login(user: String, password: String) -> Result<String, String> {
    if user == "tauri" && password == "tauri" {
        // resolve
        println!("You are logged in! {} {}", user, password);
        Ok("logged_in".to_string())
    } else {
        // reject
        println!("You are not logged in! {} {}", user, password);
        Err("invalid user info".to_string())
    }
}

#[derive(serde::Serialize)]
struct CustomResponse {
    message: String,
    other_val: usize,
}

async fn some_other_function() -> Option<String> {
    Some("response".into())
}

#[tauri::command]
async fn my_custom_command(number: usize, ) -> Result<CustomResponse, String> {
    let result: Option<String> = some_other_function().await;
    if let Some(message) = result {
        Ok(CustomResponse {
            message,
            other_val: 42 + number,
        })
    } else {
        Err("No result".into())
    }
}

#[tauri::command]
async fn create_table() -> Result<i32, String> {
    let connd = query::create_conn();
    if let Err(_e) = connd {
        println!("Create Connect Error");
        return Err("Create Connect Error".to_string());
    }
    let mut conn = connd.unwrap();

    let mut res = query::create_table(&mut conn);
    if let Err(_e) = res {
        println!("Create Table Error");
        return Err("Create Table Error".to_string());
    }

    // データの挿入
    // 1.insertする構造体作成
    let users = vec![
        User { id: None , name: "Alice".to_string(), age: 30 },
        User { id: None,  name: "Bob".to_string(), age: 25 },
    ];

    // 2.insertする関数に構造体の配列を渡す
    // 参照渡しで渡すことで、関数内で変更した内容が呼び出し元にも反映されるようにする
    res = query::insert_users(&mut conn, &users);
    if let Err(_e) = res {
        println!("Insert Table Error");
        return Err("Insert Table Error".to_string());
    }

    return Ok(0);
}

#[tauri::command]
async fn insert_table(insets: Vec<query::User>) -> Result<i32, String> {
    let connd = query::create_conn();
    if let Err(_e) = connd {
        println!("Create Connect Error");
        return Err("Create Connect Error".to_string());
    }
    let mut conn = connd.unwrap();
    query::insert_users(&mut conn,insets.as_slice())
        .expect("Insert Table Error");

    Ok(0)
}

#[tauri::command]
async fn select_table() -> Result<Vec<query::User>, String> {
    let connd = query::create_conn();
    if let Err(_e) = connd {
        println!("Create Connect Error");
        return Err("Create Connect Error".to_string());
    }
    let mut conn = connd.unwrap();

    let users: Vec<User> = query::select_users(&mut conn).expect("Select Error");

    return  Ok(users);
}