# SQL ServerについてTips

[コマンドライン接続/csv出力](#connect)
[アタッチ/デタッチ](#attach)
[パフォーマンスを調査する](./performance.md)

---
---

## <a name=connect>コマンドライン接続/csv出力</a>

```bat
ヘルプ オプション表示
sqlcmd /?

サーバを指定した接続(-s, <- 区切り文字を","　-W (余分なスペースを削除))
sqlcmd -S <サーバ/インスタンス名> -H <ホスト名> \
-U <ログインID> -P <パスワード> \
-s, -W \
-i <入力するSQLファイル> -o <出力ファイル>

セキュリティ接続(userIdとかいらないはず。。)
sqlcmd -E -S <サーバ/インスタンス名> -H <ホスト名> \
-s, -W \
-i <入力するSQLファイル> -o <出力ファイル>
```

---
---

## <a name=attach>アタッチ/デタッチ</a>

- アタッチされているファイルを確認する

```sql
-- アタッチされているファイルを確認する
USE [database_name] 
GO  
SELECT type_desc, name, physical_name from sys.database_files;
```

- アタッチ/デタッチする

```sql
-- アタッチ
CREATE DATABASE MyAdventureWorks   
    ON (FILENAME = 'C:\MySQLServer\AdventureWorks_Data.mdf'),   
    (FILENAME = 'C:\MySQLServer\AdventureWorks_Log.ldf')   
    FOR ATTACH;

-- デタッチ
USE master;  
GO  
-- database_name は sysname 値
sp_detach_db [ @dbname= ] 'database_name'
    -- UPDATE STATISTIC をスキップするか実行するか
    [ , [ @skipchecks= ] 'skipchecks' ]
    -- デタッチされているデータベースに関連付けられている
    -- フルテキスト インデックス ファイルが、データベースデタッチ操作中に削除されないことを指定
    [ , [ @keepfulltextindexfile = ] 'KeepFulltextIndexFile' ]
GO
```
