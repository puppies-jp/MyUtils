# Jest

Jest(JavaScript単体テストフレームワーク)を使ってみる

- インストール方法

```sh
npm install --save-dev jest
```

- 実行方法
  - jestは`デフォルト`で`__tests__フォルダ`中のjavascritpをテストコードとして実行する

```sh
npx jest
```

## サンプル

```js
test('test 1.', ()=>{
  expect(1+2).toBe(3);
});

```

## 設定について

- 以下で設定を追加できる

1. package.jsonに記載する
2. jest.config.jsに記載する
    - `npx jest --init`でconfigが作成される
3. 実行時に--configオプションで指定したファイルに指定する

## カバレッジ

```sh
npx jest --coverage
```

## 特定の関数をモックにする

- テスト対象

```js
 function sum(a, b) {
   return a + b;
 }
 function minus(a, b) {
   return a - b;
 }
 module.exports = {
   sum : sum,
   minus : minus
 }
```

- モック化テスト
  - `spyOn`で置き換えられる。

```js
 const calc = require('../calc');

 test('test sum.', ()=> {
   expect(calc.sum(1, 2)).toBe(3);
 });
 test('特定の関数についてmockを使用して特定の値を返却する', ()=> {
   // モックを設定する
   console.log('spyOnの前', calc.sum, calc.minus);
   jest.spyOn(calc, 'sum').mockReturnValue(5);
   console.log('spyOnの後', calc.sum, calc.minus);

   // モックを呼び出す
   expect(calc.sum(1,2)).toBe(5);

   // モックを元の関数に戻す
   calc.sum.mockRestore();
   console.log('mockRestoreの後', calc.sum, calc.minus);
   expect(calc.sum(1,2)).not.toBe(5);
 });
 test('特定の関数についてmockを使用して特定の関数を実行する', ()=> {
   jest.spyOn(calc, 'sum').mockImplementation((a,b) => {
     console.log('mock function');
     return 100;
   });
   expect(calc.sum(1,2)).toBe(100);
   jest.restoreAllMocks();
   expect(calc.sum(1,2)).toBe(3);
 });
```
