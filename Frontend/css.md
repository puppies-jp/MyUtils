# cssについて

- [id/class/要素 を指定した書き方について](#tag)
- [変数(ぽいもの)を扱ってみる](#valiable)
- [keyframe](#keyframe)

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

- [sample](src/css/test1.html)

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

## <a name=keyframe>keyframeについて</a>

```css
@keyframes bnr-animation{
  0% {width: 60px; background-color: #6cb371; }
  50% {width: 234px; height: 60px; background-color: #ffd700; }
  100% {width: 234px; height: 234px; background-color: #ff1493; }
}

@keyframes animation{
  from{
    /*アニメーションを開始するときのCSSを記述*/
  }

  to{
    /*アニメーションを終了するときのCSSを記述*/
  }
}

.box{
  width: 60px; height: 60px; background: #6cb371;
  animation-name: bnr-animation;
  animation-duration: 10s;}
```
