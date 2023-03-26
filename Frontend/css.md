# cssについて

- [id/class/要素 を指定した書き方について](#tag)
- [変数(ぽいもの)を扱ってみる](#valiable)

## <a name=tag>id/class/要素 を指定した書き方について</a>

```css
.<class名>{}
/*
.nameClass {color: red;}
これもできる
<タグ名>.<class名> {color : black}
*/

#<id名> {}
/* 
#nameId {color: blue;}
<タグ名>#<id名> {color : black}
*/

<タグ名> {}
/*
div { color: yellow;}
*/

/* 複数指定　は以下(タグ、id,classなんでもOKらしい) */
◯◯,xx,△△ {

}
```

---
---

## <a name=valiable>変数ぽいものを扱ってみる</a>

- [sample](src/css/text1.html)

```html

<html>
<head>
  <style>
    :root {
        --color: red;
      }
      
      div {
        color: white;
        background-color: var(--color)
      }
      
  </style>
</head>

<body>
  <!-- デフォルトの赤が表示される。 -->
  <div>赤</div>
  <!-- 指定した色が表示される。 -->
  <div style="--color: blue;">青</div>
  <div style="--color: gold;">金</div>
  <!-- 
    🚨未指定の場合、赤が表示される。(--colorが書き換わる訳ではない!!)
    -->
  <div>赤</div>
</body>

</html>
```
