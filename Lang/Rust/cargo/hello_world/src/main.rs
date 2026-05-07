use std::string::ToString;
use std::vec;
use mysql::*;
use mysql::prelude::*;

#[derive(Debug, Clone,PartialEq)]
struct Payment {
    customer_id: i32,
    amount: i32,
    account_name: String,
}

pub struct MySQLConnect {
    host : String,
    port : i32,
    usr : String,
    pass : String,
    database : String,
}

impl MySQLConnect {
    pub fn new(host: String,port : i32,usr:String, pass: String,database:String) -> Self{
        Self{ host ,port,usr, pass,database }
    }

    fn make_url(&self) -> String {
        format!("mysql://{0}:{1}@{2}:{3}/{4}"
                ,self.usr,self.pass,self.host,self.port,self.database)
    }

    pub fn make_connect(&mut self) -> Result<mysql::PooledConn, String>
    {
        let url = self.make_url();
        let pool = mysql::Pool::new(url.as_str());
        if let Err(ref e) = pool {
            println!("Failed to make pool: {}", e);
            return  Err("make Pool Error".to_string())
        }

        let a = pool.expect("Get Error");
        let conn  = a.get_conn();
        match conn {
            Ok(num) => {
                println!("Connected!!");
                let con = self.make_table(num);
                Ok(con)
            },
            Err(ref e) =>{
                let err = format!("Failed to connect: {}", e);
                println!("{0}",err.to_string());
                Err(err)
            }
        }
    }

    pub fn make_table(&mut self,mut connect: mysql::PooledConn) -> mysql::PooledConn{
        connect.query_drop(
            r"CREATE TEMPORARY TABLE payment (
            customer_id int not null,
            amount int not null,
            account_name text
        )");
        return  connect;
    }

    pub fn select_table(self,res:&mut Vec<Payment> ,connect :&mut mysql::PooledConn) 
    {
        let res = connect
            .query_map(
                "SELECT customer_id, amount, account_name from payment",
                |(customer_id, amount, account_name)| {
                    let p = Payment { customer_id, amount, account_name };
                    res.push(p);
                },
            );
    }


}

fn main() -> std::result::Result<(), Box<dyn std::error::Error>> {
    let host  = "localhost".to_string();
    let usr = "admin".to_string();
    let pass = "password".to_string();
    let port:i32 = 53306;
    let database = "mydatabase".to_string();

    let mut connect :MySQLConnect = MySQLConnect::new(host,port,usr,pass,database);
    let mut conn = connect.make_connect()?;
    // Let's create a table for payments.

    let payments = vec![
        Payment { customer_id: 1, amount: 2, account_name: "None".to_string() },
        Payment { customer_id: 3, amount: 4, account_name: "foo".to_string() },
        Payment { customer_id: 5, amount: 6, account_name: "None".to_string() },
        Payment { customer_id: 7, amount: 8, account_name: "None".to_string() },
        Payment { customer_id: 9, amount: 10, account_name: "bar".to_string() },
    ];

    // Now let's insert payments to the database
    conn.exec_batch(
        r"INSERT INTO payment (customer_id, amount, account_name)
          VALUES (:customer_id, :amount, :account_name)",
        payments.iter().map(|p| params! {
            "customer_id" => p.customer_id,
            "amount" => p.amount,
            "account_name" => &p.account_name,
        })
    )?;

    let mut selected_payments : Vec<Payment> = Vec::new();
    // Let's select payments from database. Type inference should do the trick here.
    connect.select_table(&mut selected_payments,&mut conn);

    let mut count = 0;
    println!("First");
    for temp in selected_payments {
        println!("Name: {0}/ Id:{1}", temp.account_name, temp.customer_id);
        assert_eq!(payments[count], temp);
        count = count +1;
    }

    println!("Second");
    for temp in payments {
        println!("Name: {0}/ Id:{1}", temp.account_name, temp.customer_id);
    }

    Ok(())
}
