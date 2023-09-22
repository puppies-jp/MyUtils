# SQLのindexについて

インデックスとは、データの検索速度を向上させるために、どの行がどこにあるかを示した索引のことです。 データを検索するときに、目的のデータが見つかるまですべての行を一行ずつ調べていくよりも、索引を利用して目的の行の場所を見つけてからその行のデータを読み取る方が効率的だという考えにより、非常によく用いられる方法です。

```sql
/* 1テーブルに対して複数のインデックスを作ることもできる(NONCLUSTEREDだけ) */
CREATE NONCLUSTERED INDEX [IdxNo] ON [dbo].[Person] ([No])
CREATE NONCLUSTERED INDEX [IdxName] ON [dbo].[Person] ([Name])
CREATE NONCLUSTERED INDEX [IdxAge] ON [dbo].[Person] ([Age])

/* UNIQUE 制約を持つ非クラスター化インデックスを作成し、並べ替え順序を指定する */
CREATE UNIQUE INDEX index1 ON schema1.table1 (column1 DESC, column2 ASC, column3 DESC);

/* withでヒントを与えることで参照するインデックスを指定することもできる */
SELECT * FROM [dbo].[Person]
SELECT * FROM [dbo].[Person] WITH(INDEX ([IdxNo])) WHERE [No] = 1
SELECT * FROM [dbo].[Person] WITH(INDEX ([IdxName]))WHERE [Name] = 'Tanaka'
SELECT * FROM [dbo].[Person] WITH(INDEX ([IdxAge]))WHERE [Age] > 30
```

---
---

## クラスタ化/非クラスタ化 の違い

### クラスタ化インデックス

1. クラスター化インデックスは、テーブルまたはビュー内のデータ行をそのキー値に基づいて並べ替え、格納します。  
    `1 つのテーブルに設定できるクラスター化インデックスは 1つだけです。`  

### 非クラスタ化インデックス

1. データ行とは独立した構造になっています。 非クラスター化インデックスには、`非クラスター化インデックスのキー値が含まれており、各キー値のエントリにはキー値が含まれているデータ行へのポインターが含まれています。`

---
---

## 複合indexの注意点

複合インデックスの場合

**インデックスのキー値を複数列設定した複合インデックスの場合、検索条件の順番に注意する必要があります。**

`2つの列をインデックスのキー値として設定した場合`、1番目の列のみ、もしくは両方の列を検索条件に指定した場合、`インデックスは使用されます。`
しかし、**`2番目の列のみを検索条件に指定してもインデックスは使用されません。`**

---
---

## 断片化率計測方法

これでいけるらしい。。。

```sql
SELECT 
    SCM.name AS schema_name,
    OBJCT.name AS table_name,
    IDX.name AS index_name,
    IPS.avg_fragmentation_in_percent,
    IPS.fragment_count,
    IPS.avg_fragment_size_in_pages,
    IPS.page_count,
    IPS.avg_page_space_used_in_percent,
    IPS.record_count,
    IDX.type_desc,
    IDX.is_primary_key,
    IDX.is_unique,
    IDX.is_disabled
FROM sys.dm_db_index_physical_stats (DB_ID(),null,null,null,'DETAILED') AS IPS 
LEFT OUTER JOIN  sys.objects AS OBJCT 
    ON  IPS.object_id = OBJCT.object_id 
LEFT OUTER JOIN  sys.indexes AS IDX 
    ON  IPS.object_id = IDX.object_id  AND IPS.index_id = IDX.index_id 
LEFT OUTER JOIN  sys.schemas AS SCM 
    ON  OBJCT.schema_id = SCM.schema_id 
WHERE OBJCT.type = 'U'
    AND   IDX.index_id > 0 
AND   IPS.avg_fragmentation_in_percent > 30 
ORDER BY IPS.avg_fragmentation_in_percent DESC; 
```

DB単位にするにはこれ

```sql
USE [DB名]

DECLARE @DB_ID int
DECLARE @OBJECT_ID int

SET @DB_ID = DB_ID('DB名')
SET @OBJECT_ID = OBJECT_ID('テーブル名')

SELECT *
FROM sys.dm_db_index_physical_stats(@DB_ID, @OBJECT_ID, null, null, 'DETAILED') AS A
JOIN sys.objects AS B
ON A.object_id = B.object_id
```

[qiita参考 SQL Serverのインデックスの理解を深める](https://qiita.com/fuk101/items/2e6a225a97a14f0f2850#%E3%82%A4%E3%83%B3%E3%83%87%E3%83%83%E3%82%AF%E3%82%B9%E3%81%AE%E6%96%AD%E7%89%87%E5%8C%96%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6)

---
---

## <a name=rebuild>再構築/再構成</a>

```sql
--再構築(作り直し 再構築がひとつのトランザクションでインデックスを完全に再作成する)
ALTER INDEX index_name
ON <Table名,スキーマ名,ビュー名>
REBUILD

--再構成( 再構成は処理単位ごと (処理単位はページ) にトランザクションが分割されていて、リーフページ間でインデックス行を移動させることで行を前に詰めて断片化を解消している)
ALTER INDEX index_name
ON <Table名,スキーマ名,ビュー名>
REORGANIZE
```
