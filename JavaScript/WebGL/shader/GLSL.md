# GLSLの構文についてまとめる

GLSLについては詳しくないのでkeywordからまとめていく

- uniform変数
  - グローバル変数を入れる
  - アプリから情報を受け取る

- attribute変数
  - 頂点情報などを入れる
  - 「頂点から情報を受け取る」方法でシェーダに情報を伝えている。`(フラグメントシェーダに頂点がないので、使えないことに注意)`
  - positionはそれぞれの頂点で異なる値を保持

- varying
  - `VertexShader`から`FragmentShader`に変数を渡すときに使う

## 使い方

uniform,attribute両方とも

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
