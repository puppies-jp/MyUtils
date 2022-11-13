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
