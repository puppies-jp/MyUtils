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

---
---

## DBファイルを追加する

```sql
alter database [DB名] add file 
(NAME=N'(論理名)',FILENAME=N'FilePath',SIZE=XXXMB,FILEGROWTH=YYYMB),
(,~追加で他のファイルもあるなら~~)
TO FILEGROUP [ファイルグループ]

```

---
---

## 権限を確認する

```sql
--指定したユーザー（ロール）の権限一覧を表示する
SELECT
USER_NAME(grantee_principal_id) AS ユーザー
,OBJECT_NAME(major_id) AS オブジェクト
,permission_name as 権限名
,state_desc as 権限の状態
FROM sys.database_permissions
WHERE grantee_principal_id = USER_ID('ユーザーの名前かロールの名前')
```
