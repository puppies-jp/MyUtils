# npm

npmについてまとめる

[npm initとは](#init)
[ローカルインストール](#localinstall)

```sh
npm install <package名>

# 最新の npmに更新する場合
sudo npm install -g npm
sudo npm update -g npm
```

## <a name=node_modules>node_modulesについて</a>

適当なディレクトリで`npm install`するたびに出てくる`node_modelues`これライブラリのinstall先らしい。

んで、`-g (gloabal)`オプションをつけると。
`current(実行したフォルダ)`の代わりに`prefix folder(グローバルのフォルダ)`に突っ込んでくれるらしい

```sh
> npm help install

Operates in "global" mode, so that packages are installed into the prefix folder instead of the current working directory. See npm
       help folders for more on the differences in behavior.
```

---
---

## <a name=init>npm init使い方</a>

`npm init`とはカレントディレクトリに`package.json`を作成するコマンドこのコマンドで作成された`package.json`のあるフォルダで`npm install`などでパッケージをインストールすることで使用するパッケージを管理できる。

```bash
# 以下で実行で`package.json`に追記される
npm install パッケージ名
npm install --save パッケージ名
npm install -save パッケージ名
npm install -S パッケージ名

# 開発専用のパッケージ(リリースに含めないモジュール)としてインストールする
npm install --save-dev パッケージ名
npm install -D パッケージ名

# カレントディレクトリにインストールされているモジュールの一覧を出力
npm list
# グローバルインストールされているモジュール一覧を出力
npm list -g
```

---
---

## <a name=localinstall>パッケージをローカルインストールする</a>

[stackoverflow 参考](https://stackoverflow.com/questions/23250805/how-to-install-nodejs-project-locally-without-internet-connection/23251537#23251537)

要約すると以下の感じ(状況で適宜変更がいるかも？)

- ここまではオンラインでやる。
     1. `npm init`で`package.json`を作る。
     2. `package.json`に必要なパッケージを列挙する
     3. `npm install`で必要なパッケージをインストールする
        > 👆の作業はインストール済みの環境のnode_module内のパッケージのみであればいらないかも。。。
        > インストール済みであれば、node_module内で以下を実行
     4. `npm pack`でパッケージを固める(tgzファイルが作成される)

- ここからオフライン環境
     1. 固めたパッケージをインストール先に持っていく。
     2. `npm install <filename>`でインストールする。
