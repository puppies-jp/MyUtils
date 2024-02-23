# 便利な小ネタ

- 要素をドラッグで移動させるのに便利らしい、

[元ネタ](https://qiita.com/economist/items/d4254209330c11caff04)

```js
<img id="$img" src="https://XXX/xxx.svg" width="40" height="40">
<script>
$img.onpointermove = function(event){
    if(event.buttons){
        this.style.left     = this.offsetLeft + event.movementX + 'px'
        this.style.top      = this.offsetTop  + event.movementY + 'px'
        this.style.position = 'absolute'
        this.draggable      = false
        this.setPointerCapture(event.pointerId)
    }
}
</script>
```
