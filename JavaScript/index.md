# JavaScriptを触ってみる

- とりあえず、環境について
  - Ubuntu(on Docker(Xなどのウィンドウなし、完全にUbuntuのものだけ))
  - node.jsでやってみる

- [クラス定義](#defineClass)
- [scopeについて](#scope)
- [let/varの違い](#diff_let_var)


## 環境構築

```sh
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

## 実行方法(Hello world)

1. 適当なファイルに以下を書き込む

    ```javascript
    console.log('Hello World!');
    ```

2. `node <ファイル名.js>`で実行

3. **`以上`**
   クラスとかメイン関数とか気にしないで試せるのは楽かも？

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
```

## <a name=scope>スコープについて</a>

そのうち書く

## <a name=diff_let_var>let/var/constの違い</a>

違いは`再宣言可能`か`再代入可能`か`スコープ`に違いが出る

|\\|再代入|再宣言|スコープ|
|:--:|:--:|:--:|:--|
|var|可能|可能|ある関数内でvarを用いて宣言した変数は、その関数をどこからでも呼び出すことが可能|
|let|可能|不可|同じ関数内であってもブロックの外側からは変数を呼び出せません。|
|const|不可|不可|同じ関数内であってもブロックの外側からは変数を呼び出せません。|
