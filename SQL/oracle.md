# OracleのSQLサーバについて

ここではOracle特有のSQLとかについてまとめる

- [パーティション化](#partition)

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
-- クローズはこれ
alter pluggable database PDB01 close;
-- 削除
drop pluggable database PDB01 including datafiles;
-- 接続先変更
alter session set comtainer = <ORCLPDB>;
```

---

## <a name=partition>パーティション作成</a>

```sql
-- 
CREATE TABLE sales
(
    product_id NUMBER(4) NOT NULL,sales_date DATE,
    Customer_id VARCHAR2(40))
    PARTITION BY RANGE(sales_date)
    (
        PARTITION sales_p1 VALUES LESS THAN(TO_DATE(' 2011/04/01 ','YYYY/MM/DD')) TABLESPACE Q1,
        PARTITION sales_p2 VALUES LESS THAN(TO_DATE(' 2011/07/01 ','YYYY/MM/DD')) TABLESPACE Q2,
        PARTITION sales_p3 VALUES LESS THAN(TO_DATE(' 2011/10/01 ','YYYY/MM/DD')) TABLESPACE Q3,
        PARTITION sales_p4 VALUES LESS THAN(TO_DATE(' 2012/01/01 ','YYYY/MM/DD')) TABLESPACE Q4
    );

-- コンポジットパーティション
-- 複数keyでパーティション作成する場合
CREATE TABLE stable (
    sdate DATE /* 売り上げ日 */, 
    reg VARCHAR2(20) /* 地域 */, 
    cost NUMBER　/* 値段 */)
    PARTITION BY RANGE (sdate)/* メインPartation */
    SUBPARTITION BY LIST (reg) /* サブ */
    (
        PARTITION P2009Q1 VALUES LESS THAN(to_date('2009-04-01','YYYY-MM-DD'))
        (
            SUBPARTITION P2009Q1_kanto VALUES ('kanagawa','Tokyo'),
            SUBPARTITION P2009Q1_kansai VALUES ('Osaka','kyoto')
        ),
        PARTITION P2009Q2 VALUES LESS THAN(to_date('2009-07-01','YYYY-MM-DD'))
        (
            SUBPARTITION P2009Q2_kanto VALUES ('kanagawa','Tokyo'),
            SUBPARTITION P2009Q2_kansai VALUES ('Osaka','kyoto')
        )
    );

```

- 後から追加する場合

```sql
-- RANGEだと追加できる方向(Less thanとかの大きさ)が決まってるっぽい。。
ALTER TABLE stable ADD PARTITION P201005 VALUES LESS THAN (TO_DATE('2010/4/01','YYYY/MM/DD'));
-- subpartitionもこんな感じ
ALTER TABLE stable ADD PARTITION P201105 VALUES LESS THAN (TO_DATE('2011/4/01','YYYY/MM/DD'))
(SUBPARTITION P2011Q2_kanto VALUES ('kanagawa','Tokyo'));

-- 後からsubpartiotionを弄る場合(こんな感じ)
ALTER TABLE stable MODIFY PARTITION P201105 
ADD SUBPARTITION P2021Q2_kansai VALUES ('Osaka','kyoto');

```

- パーティション確認方法

```sql
SELECT TABLE_NAME FROM USER_TABLES WHERE  PARTITIONED = 'YES';

SELECT TABLE_NAME,PARTITION_NAME FROM USER_TAB_PARTITIONS;
```

---

## PDB作成

この辺のDB操作はsysユーザで行うこと

- その1

```sh
mkdir /opt/oracle/oradata/XE/pdb01
```

```sql
-- ユーザを作成
create user oracle identified by oracle; 
-- PDBの新規作成(pdb$seedから作成)
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
CREATE USER hogeuser IDENTIFIED BY passw0rd
DEFAULT TABLESPACE users
QUOTA UNLIMITED ON users
TEMPORARY TABLESPACE temp;

-- 作成ユーザへ権限付与
GRANT CREATE SESSION TO oracle;
GRANT RESOURCE TO oracle;
GRANT UNLIMITED TABLESPACE TO oracle;
```
