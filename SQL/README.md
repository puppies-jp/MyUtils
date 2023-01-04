# SQL関連まとめ

好きじゃないがめんどくさいのでここにいろいろまとめる

- [oracle](oracle)
- [sqlite3](sqlite3)

```sql
-- sqlite3 だけ？っぽい
PRAGMA table_info('テーブル名')
```

## Joinについて

left joinの場合`select * from [左テーブル] LEFT JOIN [右テーブル] ON ...`  
といった形でSQL文を作成できる

![About Join](img/SQL_JOINS.png)

## Union/Union Allについて

```sql
-- union(重複したレコードは削除される)
select test_pk, test_val from test1
union 
select test_pk, test_val from test2

-- union all(重複の削除は行わない)
select test_pk, test_val from test1
union all 
select test_pk, test_val from test2
```

## Merge(upsert)文について

近年追加された構文で1ステートメントでinsert/update/deleteを同時に実行できるSQL
ユースケースとして`upsert`,`複数のテーブルデータをマージ`するといった場合が考えられる

`insert on conflict`との違いとしては、`insert on conflict`は`insert`を主目的として競合時の処理を実行できるのに対し、
いるのに対しupdate/delete処理を選ぶことができる点がある。

```sql

```
