# React

流行りに乗ってReactについてまとめる

[画面遷移](page)

```bash
# プロジェクト作成
npx create-react-app myapp
# サーバ立ち上げ
cd myapp
npm start
```

## docker 立ち上げ方

```bash
# myappというプロジェクトを作成する
npx create-react-app myapp
# サーバ立ち上げ
cd myapp
npm start
```

```dockerfile
FROM node:16-alpine
WORKDIR  /myapp
# npxで作成したプロジェクトをコピーするため、パス、フォルダ名は適宜変更すること
COPY myapp /myapp 
EXPOSE 3000
CMD ["npm", "start"]


```

## Tagに変数をつけて実行する

```js
import React from 'react';

// Componentを継承したクラスを作る
class ShoppingList extends React.Component {
  render() {
    return (
      <div className="shopping-list">
        <h1>Shopping List for {this.props.name}</h1>
        <ul>
          <li>Instagram</li>
          <li>WhatsApp</li>
          <li>Oculus</li>
        </ul>
      </div>
    );
  }
}


function App() {
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.js</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
      // 作成したクラスにnameプロパティを追加することで変数として使える
      <ShoppingList name="Mark" />
    </div>
  );
}

export default App;
```

## stateについて

React コンポーネントはコンストラクタで this.state を設定することで、状態を持つことができるようになります。this.state はそれが定義されているコンポーネント内でプライベートと見なすべきものです。

```js
class Square extends React.Component {
  constructor(props) {
    super(props);
    // コンポーネントのstateを定義する
    this.state = {
      value: null,
    };
  }

  render() {
    return (
        // クリック時に値を変えるようにすることで、表示の値も変えることができる。
      <button
        className="square"
        onClick={() => this.setState({value: 'X'})}
      >
        {this.state.value}
      </button>
    );
  }
}
```
