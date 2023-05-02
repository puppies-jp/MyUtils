# JavaScriptを触ってみる

- とりあえず、環境について
  - Ubuntu(on Docker(Xなどのウィンドウなし、完全にUbuntuのものだけ))
  - node.jsでやってみる

- [モジュール管理について](npm)
  - [Electron](Electron/electron)
  - [カメラ/マイクアクセス](DeviceAccess)

- [デバッグについて](#Debug)
- [Hello World兼Json <--> str変換](#base)
- [クラス定義](#defineClass)
- [関数定義とlambda式について](#func)
- [let/varの違い](#diff_let_var)
- [非同期処理(async/await)を使ってみる](#async)
  - [クリティカルリージョンについて](#critical)
- [yieldについて](#yield)

- [モジュール作成](#module)
- [演算子のオーバーロードについて](#overload)

---
---

## 環境構築

```sh
# 🚨 aptやっぱダメだわ(笑) 
# Microsoftのやつを参考にcurlでinstallシェルとってきた方が安全かも
# nodejs,npmをinstall
apt install -y nodejs npm

# n packeageをglobalにインストール
npm install n -g

# n package を使って node をインストール
n lts

### npmの更新　###############

# npm
npm info npm version

# 最新の npmに更新
sudo npm install -g npm
sudo npm update -g npm

```

- `n`ってなんだ？
  -> node.jsのバージョン管理パッケージっぽい
  `n ls`,`n list`でインストール済みのバージョンを確認したり
  バージョン切り替えに使うっぽい

- `npm`: パッケージ管理ツールみたい(Dockerのubuntuでなんかエラー吐いてる。。)
  とりあえず、今回は導入できればいいのでエラーの調査はしない！

---
---

## <a name=base>実行方法(Hello world)</a>

1. 適当なファイルに以下を書き込む

    ```javascript
    console.log('Hello World!');

    // string <--> json 変換 //
    // string -> json
    var jdata = JSON.parse("{\"data1\":data2}");

    // json -> string
    var str = JSON.stringify(
      {"count":count,"Now":new Date()});
    ```

2. `node <ファイル名.js>`で実行

3. **`以上`**
   クラスとかメイン関数とか気にしないで試せるのは楽かも？

---
---

## <a name=Debug>Debugについて</a>

- とりあえず、これでできる。
(ツール(ChromeDevTools)を使うことでGUIでのデバッグも可)
`chrome://inspect` で開いて`Open dedicated DevTools for Node`へ行くことでデバッグできるようになるらしい。  

- breakpointはソースに`debugger;`と追記する。。。
(微妙すぎる。。)

```shell
node inspect <対象ファイル>

# 👇これでChromeDevToolsを使えるようになる
node –inspect-brk app.js
```

---
---

## <a name=defineClass>クラス定義について</a>

- とりあえず特徴的っぽいとこをあげる。
  - `new`をつけてインスタンスを作成する。
  - `constructor`は複数定義できなさそう。。

```javascript
class Animal{
    constructor (baw) {
         this.baw = baw;
         this.baw2 = "non";
        }
    // コンストラクタは2つは作れないっぽい(知らんけど、)
    /*constructor (baw,baw2) {
        this.baw = baw;
        this.baw2 = baw2;
   }*/
   say (){
        console.log(this.baw+ " " + this.baw2);
    }
}

// new をつけないとコンストラクタが動かないらしい
cat = new Animal("mew mew");
// dog = new Animal("baw","wow");

cat.say();
//dog.say();

// 🌟継承はこんな感じ
class fish extends Animal{
    constructor(name)
    { 
        //🌟親クラスのコンストラクタはこんな感じ
        super("I`m fish!");
        super.baw2 ="";
    }

    talk(){
        // 🌟親クラスの関数を実行する。
        super.say();
    }
}

jerryfihs = new fish("");
jerryfihs.talk();
```

---
---

## <a name=func>関数定義とlambda式</a>

`Python`みたいに関数をクラス一つとして扱っているっぽい

```javascript
// 通常の宣言
function hello(name){
    console.log("hello "+ name);
};

// lambda式(無名関数)
const f = function(name){
    console.log("hello "+ name+ "!");
};

// lambda式(arrow)
// (上記の変数f同様に関数クラスのように扱うこともできる。)
(num) => {
  return num * 2
}
```

---

## <a name=diff_let_var>let/var/constの違い</a>

違いは`再宣言可能`か`再代入可能`か`スコープ`に違いが出る

| *|再代入|再宣言|スコープ|
|:--:|:--:|:--:|:--|
|var|可能|可能|ある関数内でvarを用いて宣言した変数は、その変数をどこからでも呼び出すことが可能|
|let|可能|不可|同じ関数内であってもブロックの外側からは変数を呼び出せません。|
|const|不可|不可|同じ関数内であってもブロックの外側からは変数を呼び出せません。|

varでスコープを抜けても呼び出せることから察せるように、`デストラクタがないらしい、、`

つまり、リソースの使い方には注意してコーディングする必要がある。

## <a name=async>非同期処理(async/await)を使ってみる</a>

1. パッケージをインストールする
　　(パッケージもあるが、デフォでPromise型が使える)
  asyncパッケージでの使い方はそのうち

> npm install async

```javascript
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

// asyncの関数である宣言が必要みたい(C#と同じっぽい🤗)
async function myAsync() {
     
    var prom = myPromise(20);
    console.log("Waitting .. process")
 
    // 色々処理やった後で、awaitで取り出せばOK
    // C#とほぼ同じっぽい
    var result = await prom;
    console.log(result);
 
}
 
myAsync();
```

### <a name=critical>非同期処理におけるクリティカルリージョンについて</a>

jsにはmutexなどはないらしい、結論としてクリティカルリージョンは以下で実装が必要

1. `ロック用の変数用意して自分で排他処理しないとダメ `
1. `想定外のコード実行が行われないように、Arrayをタスクキューとして使って、そこに期待した順番でタスクを並べてごにょごにょすれば良い`

---

## <a name=module>モジュール作成</a>

```javascript
class Animal{};

// require('hello-world-script')でclassを作成できるようにする
module.exports = Animal;

// import xxx from 'hello-world-script'できるようにする
module.exports.default = Animal;
```

---

## <a name=overload>JavaScriptにおけるオーバーロードについて</a>

- [StackOverflowの回答](https://stackoverflow.com/questions/19620667/javascript-operator-overloading)

`JavaScript doesn't support operator overloading.`らしい。。。  
しかし、`valueOf`,`toString`をいじることで近い機能を実装することができる
(仕様的にオブジェクト指向としてどうなんだ？)

--- 

## <a name=yield>yieldについて</a>

JavaScriptにおけるyieldは関数の処理を一時的に呼び出し元に譲って、
関数の処理は待機しておくという意味
nextで処理を再開することができる

```js
// yieldが含まれるジェネレータ関数には、functionの最後にアスタリスクをつける必要があります。
function* funcA() {
    yield "Hello";
    yield "JavaScript";
    return "!!";
}

f1 = funcA();
// valueはyieldに指定した戻り値で、doneはジェネレータ関数が終了したかどうかを表す真偽値
console.log(f1.next()); // -> { value: 'Hello', done: false }
console.log(f1.next()); // -> { value: 'JavaScript', done: false }
console.log(f1.next()); // -> { value: '!!', done: true }
console.log(f1.next()); // -> { value: undefined, done: true }
```