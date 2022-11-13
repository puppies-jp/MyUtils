# JavaScriptを触ってみる

- とりあえず、環境について
  - Ubuntu(on Docker(Xなどのウィンドウなし、完全にUbuntuのものだけ))
  - node.jsでやってみる

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
