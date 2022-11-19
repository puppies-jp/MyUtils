# hook 

このページではldを利用したC/C++の関数置き換え、hookについて書いてみる

```sh
# 🌟ライブラリを作成する
g++ -fPIC -shared ./printhook.cpp -o hook.so -rdynamic -ldl -std=c++2a

# 🌟 置き換え対象の関数を使用しているバイナリを作成する
g++ sample.cpp -o sample.out

# 🌟 LD_PREALOADでライブラリのload順を指定して実行する(linux環境)
LD_PRELOAD=./hook.so ./sample.out

# 🌟 Macの場合はこれ
DYLD_INSERT_LIBRARIES=printf.dylib DYLD_FORCE_FLAT_NAMESPACE=YES  ./sample.out
```
