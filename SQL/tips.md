# SQL 小ネタ

[数列の作成方法](#sequence)

---

## <a name=sequence>数列を作る方法</a>

```sql

DECLARE @Count   INT = 100;

WITH NumberTable (SeqNo)
AS
(
     SELECT 1
     UNION ALL
     SELECT 1 + SeqNo
     FROM   NumberTable
     WHERE  SeqNo < @Count
)
SELECT  SeqNo
FROM    NumberTable
OPTION (MAXRECURSION 0);
```
