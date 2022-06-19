# MyUtils

- このリポジトリは私の備忘録を兼ねたメモをまとめるためのディレクトリです。

- Tensorflow 周りのめも(別のページのリンク)
  - [Tensorflow お勉強関連](https://puppies-jp.github.io/TensorflowDevLearn/)

---

ファイル構成

```shell
MyUtil % tree ./
./
├── Clang
│   └── README.md
├── FireWall
│   └── Firewall.md
├── Python
│   ├── index.md
│   └── pandas
│       ├── matplot.md
│       └── pandas_tuto.md
├── RegExp
│   └── regularExpression.md
├── ShellScript
│   └── index.md
├── _config.yml
├── index.md
├── keywords.md
├── sandbox
│   └── index.md
└── tensorflow
    ├── index.md
    └── makeModel.md

8 directories, 13 files
```

- [sandpox code pen](sandbox)
- [ShellScript](ShellScript)
- [Firewall リンク](FireWall/Firewall)
- [python](Python)
  - [pandas リンク](Python/pandas/pandas_tuto)
  - [matplot リンク](Python/pandas/matplot)
- [正規表現リンク](RegExp/regularExpression)
- [tensorflow](tensorflow)
- [Clang 系](Clang)
- [BinaryHack](BinaryHack)
- [Docker](Docker)
- [keyword 備忘録](keywords)
- [html](html)
  - [How to use leaf](html/leaf/leaf.html)
  - [learn css](html/LearnCss/LearnCss.html)
  - [learn1](html/learn1)
  - [read_file](html/read_loc_xml)
    - [src](https://github.com/puppies-jp/MyUtils/tree/gh-pages/html)

- html/javascript/css 勉強用リポジトリ  
  - [gh-pagesリンク](https://puppies-jp.github.io/LearnHtml/)

---

- Git使い方備忘録

```shell
# 🌟 直前に行ったcommitを取り消す
git reset --soft HEAD^

# 
```

`HEAD^`は「一つ前のコミット」のこと

- --soft
  HEADの位置のみ巻き戻す
  例：`git reset --soft HEAD^` →直前のコミットを消す

- --mixed
  HEADの位置、インデックスを巻き戻す
  オプションなしでも同じ結果になる
  例：`git reset HEAD` →インデックスの変更（addしたもの）を消す

- --hard
  HEADの位置、インデックス、作業ディレクトリを巻き戻す
  例：`git reset --hard HEAD^` →直前のコミットまですべて消す
