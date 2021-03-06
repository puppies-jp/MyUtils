# keyword 備忘録

- この MarkDown は system,os における用語をまとめるための備忘録である。

---

- clz count leading zero の略(先頭何ビットが 0 かというやつ)

- マングリング

  - C++では関数の多重定義などに対応するため関数のシンボル名に引数の型などの情報が付与される。

    ```md
    名前空間や多重定義などで同じ名前の関数が複数定義可能であり
    コンパイラは適切な関数へのリンクを行うためにそれらを区別できる情報を付与します。
    関数の名前や型をシンボルに埋め込むことをマングリングと言うそうです。
    ```

- プリプロセッサ ディレクティブ
  - プリプロセッサは C および C++ ファイルをコンパイラに渡す前の準備的な処理を実行します。 プリプロセッサを使用して、コードの条件付きコンパイル、ファイルの挿入、コンパイル時のエラー メッセージの指定、コード セクションへのコンピューター固有の規則の適用を行うことができます。
- gdb,pdb

  - 端的に言うと linux のコンソールで使える debuger

  - gdb

    ```md
    デバッガです。ブレークポイントを張ったり、ステップ実行したり、
    変数の中身を覗いたり、書き換えたり...そういうことが出来ます。
    gcc と同様、linux には標準でインストールされています。

    昔からあるツールであり、コマンドラインでの操作を行います。
    IDE のデバッガがどうしても使えない環境でのデバッグに役立ちます。
    ```

  - pdb

    ```md
    python は pdb モジュールを利用することで以下のようなデバッグ機能を利用することができます。

        1. ステップ実行
        2. ブレークポイントの設定
    ```

- ガベージコレクション（GC）

  - C#などでメモリリークが起こらないよう自動でリソースの解放が行われる仕組み
  - やっていることは知っているけど名前が出てこないので、メモっとく

---

- MFC(Microsoft Foundation Class) C++ 関連

  > シンプルなデスクトップ アプリケーションの作成にも使用できますが、複数のコントロールを使用する複雑なユーザー インターフェイスを開発する必要がある場合に最も役立ちます。 MFC を使用すると、Office スタイルのユーザー インターフェイスを持つアプリケーションを作成できます。

  - [MicroSoft 公式リンク](https://docs.microsoft.com/ja-jp/cpp/mfc/mfc-desktop-applications?view=msvc-160)
  - [MicroSoft mfc samples](https://docs.microsoft.com/ja-jp/cpp/overview/visual-cpp-samples?view=msvc-160#mfc-samples)
  - 以下サンプルになりそうなリンクを貼ってく
    - [VC++ Windows Programming (阪大の方の備忘録)(便利そう)](http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/)
