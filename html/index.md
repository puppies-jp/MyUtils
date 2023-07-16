# html/js/css について

このページでは `html`/`js`/`css`についていろいろまとめてく

---

## とりあえず最初のテンプレート

```html
<!DOCTYPE html>
<html lang="ja">
    <link rel="stylesheet" href="~~.css">
    <meta charset="utf-8">　
    <head>
        <title>OXページ</title>
    </head>
    <body>
      <!--🌟target="_blank"を付けることで新規にタブを開くか切り替えれる-->
      <!--
        🚨あくまでもリンクを新しいタブまたはウィンドウ (ユーザーのブラウザ設定による) で開くだけ
        targetだけではタブナビングのフィッシング詐欺の可能性があるため、
        rel="noopener noreferrer"のプロパティもつけること
      -->
      <a href="~~">OO</a>
      <a href="~~" target="_blank"　rel="noopener noreferrer">OOXX</a>

<div id="~~" class="~~"></div>
    </body>
</html>
<script src="https://code.jquery.com/jquery-1.9.1.js"></script>

```

---
---

## CDNについて

「CDN」はContent Delivery Networkの略で、簡単に言うとインターネット経由でファイルを配信する仕組みのこと
自身のローカルサーバにモジュールを配置することで使うこともできる

```html
<script src="https://code.jquery.com/jquery-3.3.1.js"></script>
```

---
---

## cssのクラスの継承について

```css

.flex {
    display: flex; /*横並び*/
  }
  .flex .image {
    width: 100%; /*画像サイズ指定*/
    margin: 0;
    padding: 0;
    overflow: hidden;
    position: relative;
  }

.list {
  scroll-snap-type: x mandatory;
  display: flex;
  overflow-x: auto;
  width: 100%;
  margin: auto;
}  

/*
<div class="親">
  <div class="子">
    <div class="孫"></div>
  </div>
  <div class="子">
    <div class="孫"></div>
  </div>
</div>
*/
/* このように書くことで上記の親の設定を子に適用される */
.list> li {
    scroll-snap-align: start;
    width: 100%;
    height: 100%;
    flex-shrink: 0;
  }
.list> li> img {
  max-width: 100%;
}
```
