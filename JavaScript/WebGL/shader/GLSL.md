# GLSLの構文についてまとめる

GLSLについては詳しくないのでkeywordからまとめていく

- [sample2](sample2/)
- [sample3](sample3/)

## キーワード

- uniform変数
  - グローバル変数を入れる
  - アプリから情報を受け取る

- attribute変数
  - 頂点情報などを入れる
  - 「頂点から情報を受け取る」方法でシェーダに情報を伝えている。`(フラグメントシェーダに頂点がないので、使えないことに注意)`
  - positionはそれぞれの頂点で異なる値を保持

- varying変数
  - `VertexShader`から`FragmentShader`に変数を渡すときに使う

- GPGPU
  - `General-Purpose Computing on graphics Processing Units`の略
  GPUを描画以外の計算にも利用してやろうって事
  webGL2.0が有効化され，transformFeedbackという関数が使用できるようになり頂点シェーダでの計算結果をバッファにバインドできるようになった。
  つまりこいつを使うとGPGPUができます！

## 組み込み変数一覧

|変数名|shader|使い方|
|--|--|--|
|gl_Position|vertex| 頂点の座標を格納する変数。|
|gl_FragCoord|frag| フラグメントの座標を格納する変数。|
|gl_FragColor|frag| フラグメントの色を格納する変数。|
|gl_PointSize|vertex| 描画するポイントのサイズを格納する変数。|
|gl_VertexID|vertex| 頂点のインデックスを格納する変数。|
|gl_InstanceID|?| インスタンスのインデックスを格納する変数。|
|gl_TextureCoord|?| テクスチャ座標を格納する変数。|
|gl_Normal|?| 法線ベクトルを格納する変数。|

---
---

## uniform,attributeで変数を渡すケース(各頂点で共通のパラメータを渡す)

```glsl
attribute vec3 position;
attribute vec4 color;
attribute float myArray[3];

// uniformは
uniform float utime;
varying lowp vec4 vColor;
    
void main ()    
{
  gl_Position = vec4(position, 1.0);
  vColor = color+ vec4(0.2,utime,1,1);
}
```

```js
// Get the location of the uniform variable
var myFloatLocation = gl.getUniformLocation(program, "myFloat");
// Set the value of the uniform variable
gl.uniform1f(myFloatLocation, 1.0);

// Get the location of the attribute variable
var myAttributeLocation = gl.getAttribLocation(program, "myAttribute");
// Set the value of the attribute variable
gl.vertexAttrib1f(myAttributeLocation, 1.0);

// 🌟配列を定義している場合は以下
// Get the location of the attribute variable
var myArrayLocation = gl.getAttribLocation(program, "myArray");
// Set the value of the attribute variable
gl.vertexAttrib3f(myArrayLocation, 1.0, 2.0, 3.0);
```

## Vertex毎の情報を渡すケース

```GLSL
attribute vec3 position;
attribute vec4 color;
uniform float utime;
varying lowp vec4 vColor;
// 🌟ここのfIndexに点毎のデータを渡される
attribute float fIndex;
void main ()
{
  gl_PointSize=30.0;
  //gl_Position = vec4(position+(0,utime), 1.0);
  gl_Position = vec4(position, 1.0);
  if(fIndex== 1.0) vColor = vec4(1.,0.+utime,0.,1)+color;
    else if(fIndex== 2.0) vColor = vec4(0.,1.,0.,1);
    else if(fIndex== 3.0) vColor = vec4(0.,0.,1.,1);
  }

```

```js
// 🌟準備段階（バッファ作成）
// 適当バッファ
var indexBuffer = gl.createBuffer();
// 生成したバッファをバインドする
gl.bindBuffer(gl.ARRAY_BUFFER, indexBuffer);
// 各色を定義
var indexes = [
    1.0, 2.0, 3.
];
// バインドされたバッファに色をセットする
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(indexes), gl.STATIC_DRAW);
// バインド解除
gl.bindBuffer(gl.ARRAY_BUFFER, null);


// 🌟レンダリング時
// 色のバッファをバインド
gl.bindBuffer(gl.ARRAY_BUFFER, indexBuffer);
// シェーダのアドレスを保持
var indexAddress = gl.getAttribLocation(program, "fIndex");
// 頂点属性の有効化
gl.enableVertexAttribArray(indexAddress);
// 頂点属性に頂点データを設定
gl.vertexAttribPointer(indexAddress, 1, gl.FLOAT, true, 0, 0);
```
