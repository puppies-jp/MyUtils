# C++ デストラクタによる安全なリソース解放について

- ファイルの close 忘れ、接続のクローズ、メモリの free 忘れはよくやる。。。(うっかりとか色々な理由で、)

  - 忘れるだけならまだ大丈夫だが、例外がスローされ close を呼ばずに次の処理に移ると手が追えなくなってしまう。

- [ファイルの open から close を安全に行うための仕組み](https://www.ipa.go.jp/security/awareness/vendor/programmingv1/b06_06_main.html)
