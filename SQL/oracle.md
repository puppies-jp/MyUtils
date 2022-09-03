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
CREATE TABLE “stable” (
    ”date” DATE /* 売り上げ日 */, 
    “reg” VARCHAR2(20) /* 地域 */, 
    “cost” NUMBER　/* 値段 */)
    PARTITION BY RANGE (”date”)/* メインPartation */
    SUBPARTITION BY LIST (“reg”) /* サブ */
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
ALTER TABLE ”stable” ADD PARTITION P201005 VALUES LESS THAN (TO_DATE('2010/4/01','YYYY/MM/DD'));
-- subpartitionもこんな感じ
ALTER TABLE ”stable” ADD PARTITION P201105 VALUES LESS THAN (TO_DATE('2011/4/01','YYYY/MM/DD'))
(SUBPARTITION P2011Q2_kanto VALUES ('kanagawa','Tokyo'));
```

- パーティション確認方法

```sql
SELECT TABLE_NAME FROM USER_TABLES WHERE  PARTITIONED = 'YES';

SELECT TABLE_NAME,PARTITION_NAME FROM USER_TAB_PARTITIONS;
```
