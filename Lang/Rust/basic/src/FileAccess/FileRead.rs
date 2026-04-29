use std::fs;
use std::fs::File;
use std::io::{ BufRead, BufReader , Write};

fn main() -> Result<(), Box<dyn std::error::Error>>{

    println!("File Access-----------------------------");
    println!("File Write-----------------------------");
    write_file();

    println!("File Read All-----------------------------");
    read_all()
    .expect("Failed to read file");
    
    println!("----------------------------");
    read_lines().expect("Failed to read file");

    Ok(())
}

fn write_file(){
    let filename = "foo.txt";
    let file = File::create(filename);
    if let Err(e) = file {
        println!("Failed to create file: {}", e);
        return;
    }
    
    let mut fio = file.unwrap();
    let mut s = fio.write_all(b"Hello, world!\n");
    s = fio.write_all(b"How are you?\n");

    return;
}

fn read_all() -> Result<(), Box<dyn std::error::Error>> {
    let content = fs::read_to_string("./test.txt")?;
    println!("{}", content);
    Ok(())
}

fn read_lines() -> Result<(), Box<dyn std::error::Error>> {

    let fs = File::open("./test.txt")?;
    for result in BufReader::new(fs).lines() {
        let l = result?;
        println!("{}", l);
    }
    Ok(())
}

