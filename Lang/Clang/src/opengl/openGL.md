# OpenGLについて

opneGLはGPUに対する描画処理などを実行するインタフェースを提供するライブラリである。
処理はIntel,AMD,Nvidiaなどに依存する
windowの表示には`x11`などがよく使われるが、他にも`ARM`などのライブラリ描画用にバックグラウンドでハードを操作するためのコンテキスト生成が行われてたりする。
また、ウィンドウを表示せずともGPUで描画用の処理を実行させ画像などを生成する
`offscreen rendering`などと呼ばれる手法も存在する。

そして、`GLSL`と呼ばれる言語を使うことでshader(シェーダ)としてGPU内で直接計算処理を行うこともできる。

- macではうまく動きそうにないが、linuxではEGLを使用することで`X`,`ARM`などの
  ハードに依存した箇所のコンテキスト生成を行える。

---

## 基本的な考え方について

とりあえず、行列計算である。
オブジェクトを作成し、好きな位置にずらす場合は現行の行列式を退避して追加。
その後、戻すことで手軽にオブジェクトを配置することができる

## Docker環境上でセーフティーに`offscreen rendering`する

仮想ディスプレイを起動してそこでレンダリングする。
**以上**

```sh
# こんな感じでDISPLAYを設定し、仮想ディスプレイを起動する。
export DISPLAY=:9
Xvfb $DISPLAY -screen 0 1024x768x24 &
```

---
---

```glsl
#define PI 3.141

float dis(float n) 
{ 
  return fract(sin(n)*1.50); 
}

void main () 
{
  float Id = dis(vertexId);
  float fr = dis(Id);
  float tex = texture2D(sound, vec2(fr, Id)).a * cos(Id);
  
  tex = pow(tex, 2.);
  
  float pang = vertexId;
  float view = .5 * 1.2;
  float t = (time * (fr + .1))*10.;
  float x = Id * sin(pang + t);
  float y = Id * cos(pang + t);
  
  y +=  .25 * tex * (1. - y);
  y *= .78;
  
  float sizeAfter = tex / (y + 1.);
  
  gl_Position = vec4(x, y, 0., 1.);
  gl_PointSize = 6. * sizeAfter;
  
  v_color = vec4(tex * .1 * (3. - fr), tex * .9 * cos(fr * PI), tex * 9., sizeAfter);
}
```
