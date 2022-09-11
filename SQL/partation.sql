SET MARKUP CSV ON
SET MARKUP CSV ON QUOTE OFF
SET MARKUP CSV ON DELIMITER '|' QUOTE OFF


CREATE TABLE stable (
    seldDate DATE, 
    reg VARCHAR2(20), 
    cost NUMBER)
    PARTITION BY RANGE (seldDate)
    SUBPARTITION BY LIST (reg)
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

ALTER TABLE stable ADD PARTITION P202104 VALUES LESS THAN (TO_DATE('2021/4/01','YYYY/MM/DD'));

ALTER TABLE stable ADD PARTITION P202104 VALUES LESS THAN (TO_DATE('2021/4/01','YYYY/MM/DD'))
(SUBPARTITION P2021Q2_kanto VALUES ('kanagawa','Tokyo'));

ALTER TABLE stable MODIFY PARTITION P202104 
ADD SUBPARTITION P2021Q2_kansai VALUES ('Osaka','kyoto');

insert into stable (seldDate, reg, cost) values (TO_DATE('2020/01/1','YYYY/MM/DD'),'kanagawa',100) ;
insert into stable (seldDate, reg, cost) values (TO_DATE('2020/02/1','YYYY/MM/DD'),'Tokyo',200) ;
insert into stable (seldDate, reg, cost) values (TO_DATE('2020/03/1','YYYY/MM/DD'),'kyoto',300) ;

select
    TABLE_OWNER --表の所有者
    ,TABLE_NAME --表の名前
    ,COMPOSITE --コンポジット・パーティションかどうか
    ,PARTITION_NAME --パーティション名
    ,HIGH_VALUE --境界値
    ,PARTITION_POSITION --パーティションの位置
    ,COMPRESSION --圧縮設定
    ,NUM_ROWS --データ件数
from
    ALL_TAB_PARTITIONS
WHERE
    TABLE_OWNER = 'ORACLE'
order by
    TABLE_NAME
;

select TABLE_OWNER --表の所有者
    ,TABLE_NAME --表の名前
    ,PARTITION_NAME --パーティション名
    ,SUBPARTITION_NAME -- サブパーティション名
    ,NUM_ROWS --データ件数
from
    DBA_TAB_SUBPARTITIONS
WHERE
    TABLE_OWNER = 'ORACLE'
order by
    TABLE_NAME
;

select TABLE_NAME
    ,PARTITION_NAME
    ,SUBPARTITION_NAME
    ,NUM_ROWS
from
    USER_TAB_SUBPARTITIONS
order by
    TABLE_NAME,PARTITION_NAME,SUBPARTITION_NAME
;