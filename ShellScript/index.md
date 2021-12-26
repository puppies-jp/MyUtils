# shell スクリプト、linux コマンドについてまとめる備忘録

---

## 可変長引数について

### 実装

```sh
drop2() {
  echo ${@:3}
}

drop2 1 2 3 4 5 6
# -> "3 4 5 6"と表示される

echo ${@:0}
```

### 実行結果

```sh
macbook:ShellScript user$ sh ./test.sh  12 13
3 4 5 6
12 13
```
