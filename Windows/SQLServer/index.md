# SQL Serverについて

CMD
[SQLサーバを起動/停止/一時停止/再開/再起動する](#cmd)
[ビット演算をする](#bit)

## <a name=cmd>SQLサーバを起動/停止/一時停止/再開/再起動する</a>

停止(stop)/一時停止(pause)するには以下のコマンドを`start`からそれぞれ`stop`,`pause`に置き換えるだけ。

- データベース エンジンを開始する

```powershell
# 🌟データベース エンジンの<既定のインスタンス>を開始する
net start "SQL Server (MSSQLSERVER)"
net start MSSQLSERVER

# 🌟データベース エンジンの<名前付きインスタンス>を開始する
net start "SQL Server (instancename)"
net start MSSQL$instancename
```

- SQL Server エージェントを開始

```powershell
# 🌟SQL Server の既定のインスタンスで SQL Server エージェントを開始する
net start "SQL Server Agent (MSSQLSERVER)"
net start SQLSERVERAGENT

# 🌟SQL Server の名前付きインスタンスで SQL Server エージェントを開始する
net start "SQL Server Agent (instancename)"
net start SQLAgent$instancename
```

- SQL Server Browser を開始する

```powershell
net start "SQL Server Browser"
net start SQLBrowser
```

## CMDログイン

```powershell
sqlcmd -S <host> -U <user> -P <password>

sqlcmd -S <host> -U <user> -P <password> -d <database>
```

## DB一覧表示

```powershell
select name from sys.databases;
go
```

---

## <a name=bit>ビット演算をする</a>

他ド忘れしたら追記してく

```sql
SELECT BIT_COUNT ( 0xabcdef ) as Count;
/*
abcdef = 1001 1010 1100 1101 1110 1111 
カウントが17となる
*/

SELECT BIT_COUNT ( 17 ) as Count;
/*
17 = 0001 0001
カウント2となる
*/
```
