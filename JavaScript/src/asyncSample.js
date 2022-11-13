var async = require('async');

// 使い方は↓の感じ
// 第1引数に配列もしくはオブジェクトで複数の関数処理を記述
// 第2引数の関数処理でエラーハンドリングや結果を取得
// async.メソッド名([関数, 関数, 関数・・・], 関数);

// npm install async

//複数の非同期処理を順番に実行
async.series([
    function(callback) {
      callback(null, '一番目');
    },
    function(callback) {
      callback(null, '2番目');
    },
    function(callback) {
      callback(null, '3番目');
    }
  ], function (err, result) {
    console.log(result);
  });

// 複数の非同期処理を同時に実行してすべての処理が完了した結果を取得
async.parallel([
    function(callback) {
        setTimeout(function() {
            callback(null, '3秒後の文字列');
        }, 3000);
    },
    function(callback) {
        setTimeout(function() {
            callback(null, '1秒後の文字列');
        }, 1000);
    }
],function(err, results) {
  console.log(results);
});

// 標準の async/awaitを使う(パッケージ入れなくてもOK)

function myPromise(num) {
    return new Promise(
        // 🚨resolveを引数に設定し、
        function(resolve) {
            setTimeout(function() {
                // 🚨resolveに結果を渡す。 戻り値は見てない
                resolve(num * num + 1);
                return 101; 
        }, 
        3000)
    })
}

async function myAsync() {
     
    var prom = myPromise(20);
    console.log("Waitting .. process")
 
    // 色々処理やった後で、awaitで取り出せばOK
    // C#とほぼ同じっぽい
    var result = await prom;
    console.log(result);
 
}
 
myAsync();