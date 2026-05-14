# Spring Boot

SpringBootについてまとめる
SpringBootはJavaのWebアプリケーションで多く使われるフレームワークである。


用語として以下のLayerで覚えておくといい(DB系のWebアプリなら)

[Qiita(#SpringとMySQLの接続設定、登録・更新機能の実装
)](https://qiita.com/Jackoguro/items/fd4f6a0744718fe3ba69)
1. インストラクチャ層（データベースの操作処理）
    1. Entity : データベース内のテーブル1行に対応するクラス
    2. Repository: データベースへのデータ操作を行うクラス  
    (`CRUD`などはここ)

2. ドメイン層（サービス処理）

    実際に使用する処理内容を記述、
    **ここからRepositoryを呼び出す**

    1. interfaceの作成
    2. interfaceに合わせたクラスの作成

3. アプリケーション層（ビュー、コントローラー）の設定

    1. Viewクラスの実装  
       htmlに合わせてViewクラスを生成する
    2. Controllerクラスの実装  
       以下のイメージ
       - 画面遷移(URLのマッピング)
       - データの受け渡しに関する内容を記述(Viewからドメイン層にデータを渡すみたいな。。。)


なんとなくこんな感じの建付け

- Model
- Repository
- Service
    - Interface
    - InterfaceMode
- Applicateion
    - form
    - Controller