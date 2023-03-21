# SQL ServerについてTips

[アタッチ/デタッチ](#attach)

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
    -- デタッチされているデータベースに関連付けられているフルテキスト インデックス ファイルが、データベースデタッチ操作中に削除されないことを指定
    [ , [ @keepfulltextindexfile = ] 'KeepFulltextIndexFile' ]
GO
```
