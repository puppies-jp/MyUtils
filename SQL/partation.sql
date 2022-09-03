

CREATE TABLE “stable” (
    ”date” DATE, 
    “reg” VARCHAR2(20), 
    “cost” NUMBER)
    PARTITION BY RANGE (”date”)
    SUBPARTITION BY LIST (“reg”)
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

ALTER TABLE ”stable” ADD PARTITION P200805 VALUES LESS THAN (TO_DATE('2009/4/01','YYYY/MM/DD'));