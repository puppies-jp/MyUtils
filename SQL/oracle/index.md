# OracleのSQLサーバについて

ここではOracle特有のSQLとかについてまとめる

- [パーティション周り](partition)
- [テーブルスペース](#tablespace)
- [REDOログとは](#redo)

- CSV形式で出力する設定

```sql
-- sqlplusなどで使える
SET MARKUP CSV ON
SET MARKUP CSV ON QUOTE OFF
SET MARKUP CSV ON DELIMITER '|' QUOTE OFF

-- 接続先確認
-- CDBに接続しているかPDBに接続しているかなど
show con_name;
-- PDB名確認(systemユーザーが実行できるっぽい)
select name, open_mode from v$pdbs;

-- 確認したPDBを起動
alter pluggable database <ORCLPDB> open;
alter pluggable database PDB01 open;
-- クローズはこれ
alter pluggable database PDB01 close;
-- 削除
drop pluggable database PDB01 including datafiles;
-- 接続先変更
alter session set comtainer = <ORCLPDB>;
```

---
---

## ユーザに与えられた権限を確認する

```sql

-- USER_SYS_PRIVSではデータベースに接続しているユーザー自身に直接付与された権限
SELECT * FROM user_sys_privs;
-- SESSION_PRIVSではセッションで使用可能な権限（ロール経由で付与されたものを含む）を確認できます。
SELECT * FROM session_privs;

-- すべてのユーザーの情報を確認するにはDBA_SYS_PRIVSに問い合わせますが、
-- このビューにアクセスするにはSELECT ANY TABLE権限が必要です。
-- この権限は、デフォルトでDBAロールに割り当てられます。
SELECT * FROM dba_sys_privs;
```

---
---

## PDB作成

この辺のDB操作はsysユーザで行うこと

- その1

```sh
mkdir /opt/oracle/oradata/XE/pdb01
```

```sql
-- 🚨ここはsysユーザで実行すること
-- PDBの新規作成(pdb$seedから作成) 作成されれば、指定したuser名でログインできる。
-- 他のuserを追加したい場合、作成したpdb内でuserを追加すること。
-- ここで作成するのぱpdbの管理ユーザ
CREATE PLUGGABLE DATABASE pdb01 ADMIN USER oracle IDENTIFIED BY oracle
file_name_convert = ('/opt/oracle/oradata/XE/pdbseed/','/opt/oracle/oradata/XE/pdb01/');

-- PDBの起動
ALTER PLUGGABLE DATABASE pdb01 OPEN;

-- PDBの自動起動設定
ALTER PLUGGABLE DATABASE pdb01 SAVE STATE;

-- PDB切り替え（これがないと、コンテナ再起動時に「ORA-01109: database not open」が発生）
ALTER SESSION SET CONTAINER=pdb01;

-- ロール権限付与 
GRANT DBA TO PDB_DBA;

-- 表領域作成
CREATE TABLESPACE users
DATAFILE '/opt/oracle/oradata/XE/pdb01/users01.dbf' SIZE 300M
AUTOEXTEND ON NEXT 500K MAXSIZE UNLIMITED;
```

---

```sql
-- PDBへ切り替え
ALTER SESSION SET CONTAINER=pdb01;

-- ユーザ削除
DROP USER hogeuser CASCADE;

-- ユーザ作成
CREATE USER oracle IDENTIFIED BY oracle
DEFAULT TABLESPACE users
QUOTA UNLIMITED ON users
TEMPORARY TABLESPACE temp;

-- 作成ユーザへ権限付与
GRANT CREATE SESSION TO oracle;
GRANT RESOURCE TO oracle;
GRANT UNLIMITED TABLESPACE TO users;
```

## <a name=tablespace>テーブルスペースとは</a>

「テーブルはどこに格納されるか？」-> `表領域(tablespace)`に格納される
表領域の実態は`ooxx.dbf`というファイル。
表領域という実態のDBFファイルの中にテーブルやデータが論理情報として保管されています。

また、表領域には3つのタイプがあります。

1. 永続的  
    永続表領域を使用して、ユーザーおよびアプリケーション・データを格納します。Oracle Databaseでは、永続表領域を使用して、システム・データなどの永続データが格納されます。各ユーザーにデフォルトの永続表領域が割り当てられます。

1. UNDO(トランザクションとかで使うやつ)  
    自動UNDO管理モードで実行されているデータベースは、UNDO表領域にUNDOデータを透過的に作成し、管理します。Oracle Databaseでは、トランザクションのロールバック、読取り一貫性、データベース・リカバリ、およびOracle Flashback Queryなどの機能を提供する目的でUNDOデータが使用されます。データベース・インスタンスが持つことができるアクティブなUNDO表領域は1つのみです。

1. temp  
    一時表領域は、SQL文によるソート処理の実行時に作成されるような一時的なデータの保存に使用されます。Oracle Databaseは、データベースの作成時に一時表領域を確保します。一時表領域グループを作成する場合は、さらに一時表領域を作成します。一般的な状況では、追加の一時表領域を作成する必要はありません。`極度に大きなデータベースがある場合に、追加の一時表領域を構成することをお薦めします。`

## <a name=redo>REDOログとは</a>

REDOログはデータベースの「更新履歴ログ」クラッシュリカバリなどで使う
`dbfファイル`に書き込むのは`チェックポイント`が発生したタイミングで書き込まれる。コミットでは一時的に`redo`ログに書き込まれるらしい

`チェックポイント`の発生契機はログファイルの閾値超過などがあり
小さいとチェックポイントが多発しパフォーマンスが低下する。

サイズの決め方は2通りらしい

- ある程度大きくとっておき、チューニング項目として拡張できる余地を残したディスク設計にしておく
- REDOログ生成量から`多くとも20回`となるようにサイズを決める
