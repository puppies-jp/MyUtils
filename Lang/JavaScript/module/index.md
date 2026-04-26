# JavaScript モジュール追加

モジュールの追加方法についてまとめる

```javascript
const greeting = "Hello, World!";

function sayHello(name) {
  return `Hello, ${name}!`;
}

class Greeter {
  constructor(name) {
    this.name = name;
  }
  greet() {
    return `Hello, ${this.name}!`;
  }
}

// 変数/関数のExpoort
export { greeting, sayHello, Greeter };

// import(*使う側でやること)
import { greeting, sayHello, Greeter } from './moduleName';

```


- `export`と`export default `は両立でできることに注意

```js
// moduleName.js
export const greeting = "Hello, World!";
export default function sayHello(name) {
  return `Hello, ${name}!`;
}
```


```js
// 他のファイルでのインポート
import sayHello, { greeting } from './moduleName';
```