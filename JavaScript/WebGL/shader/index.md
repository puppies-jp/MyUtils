# shaderについて書いてみる

- [GLSLについて](GLSL)
- [sample](sample)

## shaderとは

Shaderとは、描画を行うプログラムの総称で以下があるらしい

- オブジェクトの頂点の座標変換を行うVertex Shade
- 頂点の個数や関係を変化させてメッシュを変換させるGeometry Shader
- 画面、もしくはオブジェクト表面のピクセル色情報を操作して，色付けや描画を行うFragment Shader

## nodeでhttpサーバを立ち上げる

```bash
# これでhttp-serverを実行できる
npm install -g http-server

# これを叩くことでhttpサーバが起動する
http-server
```
