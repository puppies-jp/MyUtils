
fn main() -> Result<(), Box<dyn std::error::Error>>{

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

    println!("Match and if let-------------------------");
 
    error_example();
    println!("unwrap and except -----------------------");
    error_example2();
    println!("----------------------------");
    Ok(())
}

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
}
 

fn error_example2() {
    //let num_str = "10";
    let num_str = "10fa";
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

