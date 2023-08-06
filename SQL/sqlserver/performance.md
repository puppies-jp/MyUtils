# SQL Serverのパフォーマンスについてまとめる

何かとSQLServerで実行計画だのCPUコストだので悩まされるので、メモしておく

- [Qiita 参考ページ](https://qiita.com/fuk101/items/00eb7963ed44988b5cf1)

システム動的管理ビュー

> 動的管理ビュー (DMV) と動的管理機能 (DMF) は、サーバー インスタンスの正常性の監視、問題の診断、パフォーマンスの調整に使用できるサーバー状態情報を返します。

- sys.dm_exec_query_stats

> **キャッシュされたクエリ プランの集計パフォーマンス統計**を SQL Serverに返します。 このビューには、キャッシュされたプラン内のクエリ ステートメントごとに 1 行が含まれており、その行の有効期間はプラン自体に関連付けられています。 つまり、プランがキャッシュから削除されると、対応する行もこのビューから削除されます。

- sys.dm_exec_sql_text

> 指定された sql_handle によって識別される SQL バッチのテキストを返します。 このテーブル値関数は、システム関数 fn_get_sql に代わるものです。

- sys.dm_exec_query_plan

> プラン ハンドルで指定されたバッチのプラン表示を XML 形式で返します。 プラン ハンドルで指定するプランは、キャッシュ内のもの、または現在実行中のものを指定できます。  
> プラン表示の XML スキーマは公開され、この Microsoft Web サイト で使用できます。 また、SQL Server がインストールされているディレクトリからも入手できます。

---
---

## 過去に実行したクエリの直近100件を表示する

```sql
SELECT TOP 100
  qs.last_execution_time
 ,st.text as query_text
 ,qp.query_plan
FROM
 sys.dm_exec_query_stats as qs
CROSS APPLY
 sys.dm_exec_sql_text(qs.sql_handle) as st
CROSS APPLY
 sys.dm_exec_query_plan(qs.plan_handle) as qp
ORDER BY
 qs.last_execution_time desc
```

---

## 平均CPUコストの高いクエリの上位100件を表示する

```sql
SELECT TOP 100
  qs.last_execution_time
 ,st.text as query_text
-- ,qs.total_worker_time(totalいる？)
 ,qs.execution_count
 ,qs.total_worker_time / qs.execution_count  as avg_cpu_cost
 ,qp.query_plan
FROM
 sys.dm_exec_query_stats as qs
CROSS APPLY
 sys.dm_exec_sql_text(qs.sql_handle) as st
CROSS APPLY
 sys.dm_exec_query_plan(qs.plan_handle) as qp
ORDER BY
 avg_cpu_cost desc

```

---

## I/Oの合計数が多いクエリの上位100件を表示する

```sql
SELECT TOP 100
  qs.last_execution_time
 ,st.text as query_text
 -- 論理読み取りの合計数 /　実行回数
 ,qs.total_logical_reads / qs.execution_count as avg_logical_reads
 -- 論理書き込みの合計数 /　実行回数
 ,qs.total_logical_writes / qs.execution_count as avg_logical_writes
 ,qs.total_logical_reads + qs.total_logical_writes as total_io
 ,qs.execution_count
 ,qp.query_plan
FROM
 sys.dm_exec_query_stats as qs
CROSS APPLY -- 正直 CROSS APPLYの使い方がよくわからん、(Joinみたいな感じっぽい)
 sys.dm_exec_sql_text(qs.sql_handle) as st
CROSS APPLY
 sys.dm_exec_query_plan(qs.plan_handle) as qp
ORDER BY
 qs.total_logical_reads + qs.total_logical_writes desc
```

---
---
