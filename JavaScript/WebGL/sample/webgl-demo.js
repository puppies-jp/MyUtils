main();

//
// ここから開始
//
function main() {
  const canvas = document.querySelector("#glcanvas");
  // GL コンテキストを初期化
  const gl = canvas.getContext("webgl");

  // WebGL が使用可能で動作している場合にのみ続行
  if (gl === null) {
    alert(
      "WebGL を初期化できません。ブラウザーまたはマシンが対応していない可能性があります。",
    );
    return;
  }

  // クリアカラーを黒に設定し、完全に不透明する
  gl.clearColor(0.0, 0.0, 0.0, 1.0);
  // 指定されたクリアカラーでカラーバッファーをクリアする
  gl.clear(gl.COLOR_BUFFER_BIT);
}
