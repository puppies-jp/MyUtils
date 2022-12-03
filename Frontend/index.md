# フロントエンド

このページではフロントエンド周りについてまとめる

- [html](html)
- [css](css)

- よく使う系スニペット

```html
<!DOCTYPE html>
<html lang="ja">
    <link rel="stylesheet" href="~~.css">
    <meta charset="utf-8">　
    <head>
        <title>OXページ</title>
    </head>
    <body>
        <div id="~~" class="~~"></div>
    </body>
</html>
<script src="https://code.jquery.com/jquery-1.9.1.js"></script>
<script type="text/javascript"></script>
```

```js
// tag内データ取得
xxx=document.getElementById("ID名");
yyy=document.getElementByTagName("タグ名");

// XMLファイル読み込み系操作
var document_obj = dom_parser.parseFromString(data, "text/xml");
doc = document_obj;
let arr =document_obj.documentElement.getElementsByTagName("タグ名")
```
