# ORACLE環境のパーティションについてまとめる

- [USERの使えるテーブル、パーティション確認方法](config)
- [PARTITIONを指定しての操作](#ope)
- [パーティションの一括削除(切り捨て,truncate)](#truncate)

---

## <a name=create>パーティションを指定したtable作成</a>

```sql
-- 普通のpartitionの場合
CREATE TABLE sales
(
    product_id NUMBER(4) NOT NULL,sales_date DATE,
    Customer_id VARCHAR2(40))
    PARTITION BY RANGE(sales_date)
    (
        PARTITION sales_p1 VALUES LESS THAN(TO_DATE(' 2011/04/01 ','YYYY/MM/DD')),
        PARTITION sales_p2 VALUES LESS THAN(TO_DATE(' 2011/07/01 ','YYYY/MM/DD')),
        PARTITION sales_p3 VALUES LESS THAN(TO_DATE(' 2011/10/01 ','YYYY/MM/DD')),
        PARTITION sales_p4 VALUES LESS THAN(TO_DATE(' 2012/01/01 ','YYYY/MM/DD'))
    );

-- コンポジットパーティション
-- 複数keyでパーティション作成する場合
CREATE TABLE stable (
    sdate DATE /* 売り上げ日 */, 
    reg VARCHAR2(20) /* 地域 */, 
    cost NUMBER /* 値段 */)
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
ALTER TABLE stable 
ADD PARTITION P201105 
VALUES LESS THAN (TO_DATE('2011/4/01','YYYY/MM/DD'))
    (
        SUBPARTITION P2011Q2_kanto VALUES ('kanagawa','Tokyo')
    );

-- 後からsubpartiotionを弄る場合(こんな感じ)
ALTER TABLE stable MODIFY PARTITION P201105 
ADD SUBPARTITION P2021Q2_kansai VALUES ('Osaka','kyoto');

```

---

## <a name=config>USERの使えるテーブル、パーティション確認方法</a>

🚨 NUM_ROWSはDBMS_STATSパッケージを使用して表の統計情報を収集した場合にかぎり、移入されるらしい

```sql
SELECT TABLE_NAME FROM USER_TABLES WHERE  PARTITIONED = 'YES';

-- partitionは以下
SELECT TABLE_NAME,PARTITION_NAME FROM USER_TAB_PARTITIONS;
SELECT TABLE_NAME,PARTITION_NAME,NUM_ROWS FROM USER_TAB_PARTITIONS;

-- subpartitionはこんな感じ
SELECT TABLE_NAME,PARTITION_NAME,SUBPARTITION_NAME,NUM_ROWS FROM USER_TAB_SUBPARTITIONS;
```

---

## <a name=ope>PARTITIONを指定しての操作</a>

- `PARTITION`,`SUBPARTITION`で指定するらしい例題は`SELECT`だが、他のCRUD系操作もできるはず。

```sql
-- table内のpartitionの指定はこんな感じ
SELECT * FROM stable PARTITION(P2009Q1);
SELECT * FROM stable SUBPARTITION (P2009Q2_KANTO);

-- Docker環境でうまく動かない。。(実機は知らんが)　もしかしてMySQLの構文？
SELECT * FROM stable PARTITION(P2009Q1,P2009Q2);
```

---

## <a name=truncate>パーティションの一括削除</a>

- [参考公式](https://docs.oracle.com/cd/E57425_01/121/VLDBG/GUID-08DC6C40-64B8-4932-8FB6-E22BDC8F842B.htm)

```sql

ALTER TABLE <table名> TRUNCATE PARTITION <パーティション名> ;
ALTER TABLE <table名> TRUNCATE PARTITION <サブパーティション名> ;


-- DROP(省略可)/REUSE　を指定することができる。
-- DROP:割り当て済みの領域を削除(他のスキーマとかが使える)
-- RESUSE:割り当て済みの領域を保持
ALTER TABLE <table名> TRUNCATE PARTITION <パーティション名> DROP STORAGE;
ALTER TABLE <table名> TRUNCATE PARTITION <パーティション名> REUSE STORAGE;
```
