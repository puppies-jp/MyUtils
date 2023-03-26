# htmlについて

このページでは `html`/`js`/`css`についていろいろまとめてく

---

## とりあえず最初のテンプレート

```html
<!DOCTYPE html>
<html lang="ja">
    <link rel="stylesheet" href="~~.css">
    <!-- 🌟他ファイルにリンクせずとも直で書き込める -->
    <style>
      :root {
        --color: red;
      }
      
      div {
        color: white;
        background-color: var(--color)
      }
    </style>
    <meta charset="utf-8">　
    <head>
        <title>OXページ</title>
    </head>
    <body>
        <div id="~~" class="~~"></div>
    </body>
</html>
<script src="https://code.jquery.com/jquery-1.9.1.js"></script>
<script>
//ここにJavaScriptのコードを記述
</script>

```

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
