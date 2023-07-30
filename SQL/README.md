# SQL関連まとめ

好きじゃないがめんどくさいのでここにいろいろまとめる

- [oracle](oracle)
- [sqlite3](sqlite3)
- [SQL Server](sqlserver)

[インデックスについて](AboutIndex)
[小ネタ](tips)

```sql
-- sqlite3 だけ？っぽい
PRAGMA table_info('テーブル名')
```

---

## SQL クエリ関連について

- [Insert/Update + join/delete構文](#basic)
- [Joinについて](#join)
- [Unionについて](#union)
- [Mergeについて](#merge)
- [CASE句(条件分岐)](#CASE)
- [Like](#likeAndRegx)
- [With句](#with)

---

## <a name=basic>Select/Insert/Update/delete構文</a>

- select

```sql
言わずもがななので、省略
```

- insert構文

```sql
INSERT INTO テーブル名 (列名1, 列名2,...) VALUES (値1, 値2,...);

-- 🚨非推奨(並び変わったり、列名が変更された際に悲惨なことに)
INSERT INTO テーブル名 VALUES (値1, 値2,...);
```

- update構文

```sql
-- シンプルパターン
UPDATE (表名) SET (カラム名1) = (値1), (カラム名2) = (値2) WHERE (条件);

UPDATE employees SET title = 'Ms.'
WHERE emp_no IN (10006, 10007, 10009)

-- 🌟From を使うパターン
UPDATE  T
SET  [カラム1] = [値1],
 [カラム2] = [値1], ...
FROM  [テーブル1] AS T
WHERE  [条件];

-- Joinしてupdateするパターン
UPDATE table_A
SET table_A.medical_checkup = 'DONE'
FROM table_A
INNER JOIN table_B
ON table_A.stuff_id = table_B.stuff_id
AND table_B.checkup_date IS NOT NULL
Where table_B.scheup_data = ~~
;
```

- Delete構文

```sql
Delete from テーブル名 where 条件 
```

---
---

## <a name=join>Joinについて</a>

left joinの場合`select * from [左テーブル] LEFT JOIN [右テーブル] ON ...`  
といった形でSQL文を作成できる

![About Join](img/SQL_JOINS.png)

---
---

## <a name=union>Union/Union Allについて</a>

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

---

## <a name=merge>Merge(upsert)文について</a>

近年追加された構文で1ステートメントでinsert/update/deleteを同時に実行できるSQL
ユースケースとして`upsert`,`複数のテーブルデータをマージ`するといった場合が考えられる

`insert on conflict`との違いとしては、`insert on conflict`は`insert`を主目的として競合時の処理を実行できるのに対し、
いるのに対しupdate/delete処理を選ぶことができる点がある。

```sql
/* 🌟例1----------------------------- */
MERGE INTO /* 操作対象テーブル */user1 a
USING ( /* 参照テーブル(🌟セレクトじゃなくてテーブル指定でもOK) */
    SELECT id, name, age
    FROM user2
    ) b
ON (a.id = b.id) /*　🌟結合条件 */
WHEN MATCHED THEN /* 🌟マッチする場合 */
    UPDATE SET
        a.name = b.name,
        a.age  = b.age
WHEN NOT MATCHED THEN /* 🌟マッチしない場合 */
    INSERT (id, name, age)
    VALUES (b.id, b.name, b.age)

/* 🌟例2----------------------------- */
MERGE INTO /*🌟更新先テーブル*/ user1 a
USING (
    SELECT id, name, age
    FROM user2
) b /* 利用元テーブル */
ON (a.id = b.id)
/* 🌟マッチする場合 */
WHEN MATCHED THEN 
    UPDATE SET
        a.name = b.name,
        a.age  = b.age
-- 🌟マッチしない場合
-- BY TARGETをつけることで更新先テーブルに条件と一致するデータが存在しない場合
WHEN NOT MATCHED BY TARGET THEN
    INSERT (id, name, age)
    VALUES (b.id, b.name, b.age)
-- 🌟マッチしない場合
-- 🌟BY SOURCE をつけることで利用元テーブルに条件と一致するデータが存在しない場合
WHEN NOT MATCHED BY SOURCE THEN
    DELETE;
```

## <a name=CASE>CASE句(条件分岐)について</a>

```sql
-- 🌟こう書くことでCASE文が使える。
SELECT
    CASE
        WHEN [条件] THEN [条件を満たしたら表示する内容]
        ELSE [条件を満たしていなかったら表示する内容]
    END
FROM [テーブル名];

-- updateはこう
-- 🌟 [パターン1]80点以上の場合、2倍にして更新,80以下はそのまま
UPDATE USER
SET
    CASE WHEN 80 <= POINT THEN POINT =POINT*2 ELSE 0 END;

-- 🌟 [パターン2]80点以上の場合、2倍にして更新,80以下は0
UPDATE USER
SET
    POINT = CASE WHEN 80 <= POINT THEN POINT*2 ELSE 0 END;
```

---
---
<a name=likeAndRegx>曖昧検索と正規表現</a>

## 曖昧検索

LIKE句は、主に`曖昧検索`を行う場合に使用するクエリです。SQLでLIKE句を使用すると、対象のカラムに対して`文字列検索`をかけることが出来ます。

- 覚えるべきワイルドカード`「%」`と`「_」`

- `%`
  - 0文字以上の任意の文字列

- `_`
  - 任意の1文字

```sql
-- 前方一致
SELECT user FROM users WHERE user LIKE "山%";
-- 後方一致
SELECT user FROM users WHERE user LIKE "%山";
-- 部分一致
SELECT user FROM users WHERE user LIKE "%山%";
-- 部分一致の否定
SELECT user FROM users WHERE user NOT LIKE "%山%";
-- 完全一致
SELECT user FROM users WHERE user LIKE "山";
```

## 正規表現

- 曖昧検索だけでなく`正規表現`も使える。しかし、SQLサーバごとに使い方が違うらしい。

```sql
-- MYSQL
SELECT col_name1 [, col_name2 ...] FROM table_name
  WHERE col_name REGEXP '<正規表現パターン>'

-- PostgreSQL
SELECT col_name1 [, col_name2 ...] FROM table_name
  WHERE col_name ~ '<正規表現パターン>'

-- 🌟Oracel(REGEXP_REPLACE,REGEXP_SUBSTR,REGEXP_COUNT
-- とか便利関数があるので適宜調べること)
WHERE REGEXP_LIKE(<Column>, '正規表現パターン')

-- 🚨SQL Server(正規表現ないらしい、、、)
-- 一応 `[]`で一文字毎のはできるとか、、、(使えねー)
SELECT name FROM sys.databases
WHERE name LIKE 'm[n-z]%';/* 2文字目が n~z で引っかかる、、*/
```

## <a name="with">With句の使い方</a>

- こんな感じで副問い合わせができる

```sql
WITH 
regional_sales AS (
    SELECT region, SUM(amount) AS total_sales
    FROM orders
    GROUP BY region),
top_regions AS (
    SELECT region
    FROM regional_sales
    WHERE 
        /* 🌟ここで上のregional_salesを使う。 */
        total_sales > (SELECT SUM(total_sales)/10 FROM regional_sales)　
    )
SELECT region,
       product,
       SUM(quantity) AS product_units,
       SUM(amount) AS product_sales
FROM orders
/* 🌟ここでwith句の2つ目のクエリを実行してる */
WHERE region IN (SELECT region FROM top_regions )
GROUP BY region, product;
```
