var key = 'glcanvas';
var canvas = document.getElementById(key);
var gl = canvas.getContext('webgl', null);
var size = Math.min(window.innerWidth, window.innerHeight);
canvas.width = size;
canvas.height = size;

var vsElement = document.getElementById('vs');
var vs = null;
if (vsElement.type == 'x-shader/x-vertex') {
    vs = gl.createShader(gl.VERTEX_SHADER);
}
// シェーダオブジェクトに頂点シェーダーを代入
gl.shaderSource(vs, vsElement.text);
// 頂点シェーダをコンパイル
gl.compileShader(vs);

var fsElement = document.getElementById('fs');
var fs = null;
if (fsElement.type == 'x-shader/x-fragment') {
    fs = gl.createShader(gl.FRAGMENT_SHADER);
}
// シェーダオブジェクトにフラグメントシェーダを代入
gl.shaderSource(fs, fsElement.text);
// フラグメントシェーダをコンパイル
gl.compileShader(fs);

// WebGLProgram
var program = gl.createProgram();
function createProgram() {
    // WebGLProgramに頂点シェーダーを紐付ける
    gl.attachShader(program, vs);
    // WebGLProgramにフラグメントシェーダーを紐付ける
    gl.attachShader(program, fs);
    // WebGLProgramに紐付けられた、頂点シェーダとフラグメントシェーダをWebGLProgramにリンクさせる
    gl.linkProgram(program);
    // WebGLProgramのパラメータを取得する
    if (gl.getProgramParameter(program, gl.LINK_STATUS)) {
        // WebGLProgramを有効化する
        gl.useProgram(program);
    } else {
        console.log(gl.getProgramInfoLog(program));
    }
}
createProgram();


///三角ポリゴンの頂点バッファ生成///
// 空のバッファ生成
var positionBuffer = gl.createBuffer();
// 生成したバッファをバインドする
gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
var position = [
    0, 0.5, 0,
    -0.5, -0.5, 0,
    0.5, -0.5, 0
];
// バインドされたバッファに三角ポリゴンのデータをセットする
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(position), gl.STATIC_DRAW);
// バインド解除
gl.bindBuffer(gl.ARRAY_BUFFER, null);

// 空のカラーバッファを生成
var verticesColorBuffer = gl.createBuffer();
// 生成したバッファをバインドする
gl.bindBuffer(gl.ARRAY_BUFFER, verticesColorBuffer);
// 各色を定義
var colors = [
    1.0, 0.0, 0.0, 1.0,    // 赤
    0.0, 1.0, 0.0, 1.0,    // 緑
    0.0, 0.0, 1.0, 1.0     // 青
];
// バインドされたバッファに色をセットする
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
// バインド解除
gl.bindBuffer(gl.ARRAY_BUFFER, null);

var utime = 0.1;

// 適当バッファ
var indexBuffer = gl.createBuffer();
// 生成したバッファをバインドする
gl.bindBuffer(gl.ARRAY_BUFFER, indexBuffer);
// 各色を定義
var indexes = [
    1.0, 2.0, 3.0
];
// バインドされたバッファに色をセットする
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(indexes), gl.STATIC_DRAW);
// バインド解除
gl.bindBuffer(gl.ARRAY_BUFFER, null);


render();

function render() {
    // 背景描画
    gl.clearColor(0., 0., 0./*8*/, 1.0);
    // バッファをクリアする
    gl.clear(gl.COLOR_BUFFER_BIT);

    // ビューポートを設定
    gl.viewport(0, 0, size, size);

    // 三角ポリゴン頂点バッファをバインド
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    // 頂点シェーダのアドレスを保持
    var positionAddress = gl.getAttribLocation(program, "position");
    // 頂点属性を有効化する
    gl.enableVertexAttribArray(positionAddress);
    // 頂点属性に頂点データを設定する
    gl.vertexAttribPointer(positionAddress, 3, gl.FLOAT, false, 0, 0);

    /*
    // 色のバッファをバインド
    gl.bindBuffer(gl.ARRAY_BUFFER, verticesColorBuffer);
    // シェーダのアドレスを保持
    var vertexColorAddress = gl.getAttribLocation(program, "color");
    // 頂点属性の有効化
    gl.enableVertexAttribArray(vertexColorAddress);
    // 頂点属性に頂点データを設定
    gl.vertexAttribPointer(vertexColorAddress, 4, gl.FLOAT, false, 0, 0);
    */

    // 色のバッファをバインド
    gl.bindBuffer(gl.ARRAY_BUFFER, indexBuffer);
    // シェーダのアドレスを保持
    var indexAddress = gl.getAttribLocation(program, "fIndex");
    // 頂点属性の有効化
    gl.enableVertexAttribArray(indexAddress);
    // 頂点属性に頂点データを設定
    gl.vertexAttribPointer(indexAddress, 1, gl.FLOAT, true, 0, 0);


    var vertexTimeAddr = gl.getUniformLocation(program, "utime");
    gl.uniform1f(vertexTimeAddr, utime);
    utime = (utime + 0.2) % 1;

    //描画
    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 3);
    //gl.drawArrays(gl.LINE_LOOP, 0, 3);
    //gl.drawArrays(gl.LINE, 0, 3);

    //requestAnimationFrame(render);
}
