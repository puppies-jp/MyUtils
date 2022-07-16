# sqlite3,SQL関連

- Sqlite3では以下のようにすることでいろいろ形式を変えることができる

```sql
.header on/off
.mode csv # 🌟select時の出力がcsv形式で出力される。
.echo off
```

```sh
#!/bin/bash

function select_data () {
table=$1
table2="testdb2"
sqlite3 test.sqlite3  <<EOT 
.header off
.mode csv 
.echo off

SELECT 
'#${table} '
as '';

.header on
SELECT * 
From testdb
INNER JOIN testdb2 ON ${table}.id = testdb2.ref_id
where testdb2.name in ("hoge1","hoge2");
.header off

SELECT 
''
as '';

select 
'# ${table2}'
as '';

.header on
SELECT * 
From testdb
INNER JOIN testdb2 ON ${table2}.id = testdb2.ref_id
where testdb2.name in ("hoge1","hoge2");
.header off
EOT
}

table2="testdb"
select_data ${table2}  > test.csv

```
