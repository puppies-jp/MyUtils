# MyUtils

- このリポジトリは私の備忘録を兼ねたメモをまとめるためのディレクトリです。

- Tensorflow 周りのめも(別のページのリンク)
  - [Tensorflow お勉強関連](https://puppies-jp.github.io/TensorflowDevLearn/)

---

## 考え方系

- [オブジェクト指向について](object-oriented)
- [BinaryHack](BinaryHack)
- [keyword 備忘録](keywords)

## 言語系

- [Python🐍](Python)
- [Clang 系](Clang)
- [C# CSharp](CSharp)
  - [Blazor](CSharp/blazor)

- [Java/jsp](Java)
- [JavaScript](JavaScript)
- [html/javascript/css 勉強用](Frontend)

- [SQL](SQL)  

- [html template](html)
  - [How to use leaf](html/leaf/leaf.html)
  - [learn css](html/LearnCss/LearnCss.html)
  - [learn1](html/learn1)
  - [read_file](html/read_loc_xml)
  - [gh-pagesリンク(別ブランチ)](https://puppies-jp.github.io/LearnHtml/)

## 環境系

- [Windowsについて](Windows)
  - [WireSharkについて](WireShark)
- [WebRTC](WebRTC)
- [Git](Git)
- [Linux](Linux)
  - [ShellScript](Linux/ShellScript)
  - [Firewall リンク](Linux/FireWall/Firewall)
  - [正規表現/sed リンク](RegExp/regularExpression)

- [Docker](Docker)
- [AWS](AWS)

- [PlantUML](PlantUML)
- [sandpox code pen](sandbox)

---

## Git使い方備忘録

```shell
# 🌟 直前に行ったcommitを取り消す
git reset --soft HEAD^
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

---

## VScodeメモ

フォーマッタ起動

- mac

> Shift + Option + F

- windows

> Shift + Alt + F
