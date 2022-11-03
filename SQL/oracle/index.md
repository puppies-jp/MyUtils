# OracleのSQLサーバについて

ここではOracle特有のSQLとかについてまとめる

- [パーティション周り](partition)

- CSV形式で出力する設定

```sql
-- sqlplusなどで使える
SET MARKUP CSV ON
SET MARKUP CSV ON QUOTE OFF
SET MARKUP CSV ON DELIMITER '|' QUOTE OFF

-- 接続先確認
-- CDBに接続しているかPDBに接続しているかなど
show con_name;
-- PDB名確認(systemユーザーが実行できるっぽい)
select name, open_mode from v$pdbs;

-- 確認したPDBを起動
alter pluggable database <ORCLPDB> open;
alter pluggable database PDB01 open;
-- クローズはこれ
alter pluggable database PDB01 close;
-- 削除
drop pluggable database PDB01 including datafiles;
-- 接続先変更
alter session set comtainer = <ORCLPDB>;
```

---
---

## PDB作成

この辺のDB操作はsysユーザで行うこと

- その1

```sh
mkdir /opt/oracle/oradata/XE/pdb01
```

```sql
-- 🚨ここはsysユーザで実行すること
-- PDBの新規作成(pdb$seedから作成) 作成されれば、指定したuser名でログインできる。
-- 他のuserを追加したい場合、作成したpdb内でuserを追加すること。
-- ここで作成するのぱpdbの管理ユーザ
CREATE PLUGGABLE DATABASE pdb01 ADMIN USER oracle IDENTIFIED BY oracle
file_name_convert = ('/opt/oracle/oradata/XE/pdbseed/','/opt/oracle/oradata/XE/pdb01/');

-- PDBの起動
ALTER PLUGGABLE DATABASE pdb01 OPEN;

-- PDBの自動起動設定
ALTER PLUGGABLE DATABASE pdb01 SAVE STATE;

-- PDB切り替え（これがないと、コンテナ再起動時に「ORA-01109: database not open」が発生）
ALTER SESSION SET CONTAINER=pdb01;

-- ロール権限付与 
GRANT DBA TO PDB_DBA;

-- 表領域作成
CREATE TABLESPACE users
DATAFILE '/opt/oracle/oradata/XE/pdb01/users01.dbf' SIZE 300M
AUTOEXTEND ON NEXT 500K MAXSIZE UNLIMITED;
```

---

```sql
-- PDBへ切り替え
ALTER SESSION SET CONTAINER=pdb01;

-- ユーザ削除
DROP USER hogeuser CASCADE;

-- ユーザ作成
CREATE USER oracle IDENTIFIED BY oracle
DEFAULT TABLESPACE users
QUOTA UNLIMITED ON users
TEMPORARY TABLESPACE temp;

-- 作成ユーザへ権限付与
GRANT CREATE SESSION TO oracle;
GRANT RESOURCE TO oracle;
GRANT UNLIMITED TABLESPACE TO users;
```
