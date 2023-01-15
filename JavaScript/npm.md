# npm

npmについてまとめる

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
