use std::fs;
use std::fs::File;
use std::io::{ BufRead, BufReader};

fn main() -> Result<(), Box<dyn std::error::Error>>{
    read_all()
    .expect("Failed to read file");

    println!("----------------------------");

    read_lines().expect("Failed to read file");

    Ok(())
}


fn read_all() -> Result<(), Box<dyn std::error::Error>> {
    let content = fs::read_to_string("./test.txt")?;
    println!("{}", content);
    Ok(())
}

fn read_lines() -> Result<(), Box<dyn std::error::Error>> {
    for result in BufReader::new(File::open("./test.txt")?).lines() {
        let l = result?;
        println!("{}", l);
    }
    Ok(())
}
