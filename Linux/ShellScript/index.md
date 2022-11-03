# shell スクリプト、linux コマンドについてまとめる備忘録

---

## 数値を扱う

```sh
# $((a+b)) #こんな感じでOK
a=1;b=2
echo $((a+b))
```

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
